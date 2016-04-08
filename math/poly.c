#include "violet/math/aabb.h"
#include "violet/math/interval.h"
#include "violet/math/m2.h"
#include "violet/math/poly.h"
#include "violet/math/v2.h"

void poly_from_box(poly * p, const aabb * box)
{
	const v2 top_right = { box->bottom_right.x, box->top_left.y };
	const v2 bottom_left = { box->top_left.x, box->bottom_right.y };

	array_init(&p->vertices, sizeof(v2));
	array_reserve(&p->vertices, 4);
	array_append(&p->vertices, &box->bottom_right);
	array_append(&p->vertices, &top_right);
	array_append(&p->vertices, &box->top_left);
	array_append(&p->vertices, &bottom_left);
}

static b8 _poly_side_barycentric_contains(const poly * p, const v2 * point, u32 start_idx, u32 end_idx)
{
	const v2 * p1 = array_get(&p->vertices, start_idx);
	const v2 * p2 = array_get(&p->vertices, end_idx);
	m2 mat = { p1->x, p2->x, p1->y, p2->y };
	m2_inverse(mat, mat);
	v2 barycentric_coords;
	m2_mul(mat, point, &barycentric_coords);
	return    interval_contains(&g_0_to_1, barycentric_coords.x)
	       && interval_contains(&g_0_to_1, barycentric_coords.y)
		   && barycentric_coords.x + barycentric_coords.y <= 1;

}

b8 poly_contains(const poly * p, const v2 * point)
{
	u32 const n = p->vertices.size;
	b8 inside = _poly_side_barycentric_contains(p, point, n - 1, 0);

	u32 i = 1;
	while (!inside && i < n)
	{
		inside = _poly_side_barycentric_contains(p, point, i, i - 1);
		++i;
	}

	return inside;
}

void poly_bounding_box(const poly * p, aabb * box)
{
	const v2 center = poly_center(p);
	aabb_from_center(box, &center, &g_v2_zero);
	for (u32 i = 0, n = array_size(&p->vertices); i < n; ++i)
		aabb_extend_v(box, array_get(&p->vertices, i));
}

void poly_translate(poly * p, const v2 * delta)
{
	for (u32 i = 0, n = array_size(&p->vertices); i < n; ++i)
	{
		v2 * vertex = array_get(&p->vertices, i);
		v2_add(vertex, delta, vertex);
	}
}

interval poly_project(const poly * p, const v2 * axis)
{
	v2 unit_axis = *axis;
	if (!v2_is_unit(&unit_axis))
		v2_normalize(axis, &unit_axis);
	interval projection;
	for (u32 i = 0; i < p->vertices.size; ++i)
	{
		const v2 * vertex = array_get(&p->vertices, i);
		const r32 dp = v2_dot(vertex, &unit_axis);
		if (dp < projection.l)
			projection.l = dp;
		else if (dp > projection.r)
			projection.r = dp;
	}
	return projection;
}

v2 poly_center(const poly * p)
{
	v2 center;
	u32 n = array_size(&p->vertices);
	for (u32 i = 0; i < n; ++i)
		v2_add(&center, array_get(&p->vertices, i), &center);
	const r32 scale = 1.f / n;
	v2_scale(&center, scale, scale, &center);
	return center;
}

/*Serializer & vlt_operator<<(Serializer & serializer, const poly & poly)
{
	auto segment = serializer.create_segment(ms_segmentLabel);
	serialize_vector(*segment, poly.vertices);
	return serializer;
}

Deserializer & vlt_operator>>(Deserializer & deserializer, poly & poly)
{
	auto segment = deserializer.enter_segment(ms_segmentLabel);
	poly.vertices = deserialize_vector<v2>(*segment);
	return deserializer;
}*/

