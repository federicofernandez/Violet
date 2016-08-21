#include <assert.h>

#include "violet/math/line.h"

void POLY_(from_box)(V2 *v, const BOX2 *box)
{
	const V2 top_left = { box->min.x, box->max.y };
	const V2 bottom_right = { box->max.x, box->min.y };

	*v++ = bottom_right;
	*v++ = box->max;
	*v++ = top_left;
	*v = box->min;
}

b8 POLY_(is_simple)(const V2 *v, u32 n)
{
	assert(n >= 3);

	V2 isec;
	const V2 *a0 = v+n-1;
	u32 jend = n-1;
	for (u32 i = 0; i < n-2; ++i)
	{
		const V2 *a1 = v+i;
		const V2 *b0 = v+i+1;
		for (u32 j = i+2; j < jend; ++j)
		{
			const V2 *b1 = v+j;
			if (MATH_(segment_intersect)(a0, a1, b0, b1, &isec))
				return false;
			b0 = b1;
		}
		a0 = a1;
		jend = n;
	}

	return true;
}

b8 POLY_(is_convex)(const V2 *v, u32 n)
{
	assert(n >= 3);

	b8 cc_determined = false, cc;
	for (u32 i=0, last=n-1; i<=last; ++i)
	{
		const V2 *a = v+(i>0 ? i-1 : last);
		const V2 *b = v+i;
		const V2 *c = v+(i<last ? i+1 : 0);

		V2 ab, bc;
		V2_(sub)(b, a, &ab);
		V2_(sub)(c, b, &bc);
		const SCALAR cross = V2_(cross)(&ab, &bc);
		if (cross != 0.f)
		{
			if (!cc_determined)
			{
				cc_determined = true;
				cc = cross > 0.f;
			}
			else if ((cross > 0.f) != cc)
			return false;
		}
	}
	return true;
}

b8 POLY_(contains)(const V2 *v, u32 n, const V2 *point)
{
	BOX2 box;
	POLY_(bounding_box)(v, n, &box);
	if (!BOX2_(contains_point)(&box, point))
		return false;

	V2 out_p;
	V2_(sub)(&box.max, &box.min, &out_p);
	V2_(add)(&out_p, &box.max, &out_p);

	u32 intersections = 0;
	SCALAR t, u;
	for (u32 i = 0; i < n; ++i)
	{
		const V2 *a = v + i;
		const V2 *b = v + (i < n-1 ? i+1 : 0);
		if (   MATH_(line_intersect_coords)(a, b, &out_p, point, &t, &u)
		    && 0 < t && t <= 1 && 0 < u && u < 1)
			++intersections;
	}
	return intersections % 2 == 1;
}

void POLY_(bounding_box)(const V2 *v, u32 n, BOX2 *box)
{
	BOX2_(from_center)(box, v++, &V2G_(zero));
	for (const V2 *vn = v+n-1; v != vn; ++v)
		BOX2_(extend_point)(box, v);
}

void POLY_(translate)(V2 *v, u32 n, const V2 *delta)
{
	for (const V2 *vn = v+n; v != vn; ++v)
		V2_(add)(v, delta, v);
}

IVAL POLY_(project)(const V2 *v, u32 n, const V2 *axis)
{
	V2 unit_axis = *axis;
	if (!V2_(is_unit)(&unit_axis))
		V2_(normalize)(axis, &unit_axis);

	const SCALAR v0_proj = V2_(dot)(v++, &unit_axis);
	IVAL projection = { .l = v0_proj, .r = v0_proj };

	for (const V2 *vn=v+n-1; v!=vn; ++v)
	{
		const SCALAR dp = V2_(dot)(v, &unit_axis);
		if (dp < projection.l)
			projection.l = dp;
		else if (dp > projection.r)
			projection.r = dp;
	}
	return projection;
}

V2 POLY_(center)(const V2 *v, u32 n)
{
	V2 center = { .x=0, .y=0 };
	for (const V2 *vn = v+n; v != vn; ++v)
		V2_(add)(&center, v, &center);
	center.x /= n;
	center.y /= n;
	return center;
}

// NOTE(rgriege): Green's theorem
static
SCALAR POLY_(_area)(const V2 *v, u32 n)
{
	SCALAR area = 0;
	const V2 *prev = v+n-1;
	for (const V2 *vn = v+n; v != vn; ++v)
	{
		area += V2_(cross)(prev, v);
		prev = v;
	}
	return area/2;
}

SCALAR POLY_(area)(const V2 *v, u32 n)
{
	return ABS(POLY_(_area)(v, n));
}

V2 POLY_(centroid)(const V2 *v, u32 n)
{
	const SCALAR denom = 6 * POLY_(_area)(v, n);
	V2 centroid = { .x=0, .y=0 };
	const V2 *prev = v+n-1;
	for (const V2 *vn = v+n; v != vn; ++v)
	{
		const SCALAR cross = V2_(cross)(prev, v);
		centroid.x += (prev->x + v->x) * cross;
		centroid.y += (prev->y + v->y) * cross;
		prev = v;
	}
	centroid.x /= denom;
	centroid.y /= denom;
	return centroid;
}

b8 POLY_(is_cc)(const V2 *v, u32 n)
{
	assert(n>=3);

	SCALAR sine_sum = 0;
	for (u32 i=0, last=n-1; i<=last; ++i)
	{
		const V2 *a = v+(i>0 ? i-1 : last);
		const V2 *b = v+i;
		const V2 *c = v+(i<last ? i+1 : 0);

		V2 ab, bc;
		V2_(sub)(b, a, &ab);
		V2_(sub)(c, b, &bc);

		sine_sum += V2_(cross)(&ab, &bc) / V2_(mag)(&ab) / V2_(mag)(&bc);
	}
	return sine_sum > 0;
}

b8 POLY_(segment_intersect)(const V2 *v, u32 n,
                            const V2 *v0, const V2 *v1)
{
	const V2 *prev = v+n-1;
	for (const V2 *vn=v+n; v!=vn; ++v)
	{
		V2 isec;
		if (MATH_(segment_intersect)(prev, v, v0, v1, &isec))
			return true;
		prev=v;
	}
	return false;
}

b8 POLY_(intersect)(const V2 *p1, u32 n1, const V2 *p2, u32 n2)
{
	V2 isec;
	for (u32 i=0; i<n1; ++i)
	{
		const V2 *p1a = p1+i;
		const V2 *p1b = p1+((i+1)%n1);
		for (u32 j=0; j<n2; ++j)
		{
			const V2 *p2a = p2+j;
			const V2 *p2b = p2+((j+1)%n2);
			if (MATH_(segment_intersect)(p1a, p1b, p2a, p2b, &isec))
				return true;
		}
	}
	return false;
}

SCALAR POLY_(pt_dist)(const V2 *v, u32 n, const V2 *p)
{
	SCALAR min_dist = SCALAR_MAX;
	const V2 *prev = v+n-1;
	for (const V2 *vn=v+n; v!=vn; ++v)
	{
		const SCALAR d = MATH_(point_to_segment_dist)(prev, v, p);
		min_dist = min(min_dist, d);
	}
	return min_dist;
}

