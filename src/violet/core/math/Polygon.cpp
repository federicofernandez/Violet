#include "violet/core/math/Polygon.h"

#include "violet/core/serialization/Deserializer.h"

using namespace Violet;

namespace PolygonNamespace
{
	const char * const ms_segmentLabel("poly");
	const char * const ms_sizeLabel("n");
	const char * const ms_verticesLabel("vertices");

	std::vector<Vec2f> deserializeVertices(Violet::Deserializer & deserializer);
}

using namespace PolygonNamespace;

Polygon::Polygon(std::vector<Vec2f> vertices) :
	m_vertices(std::move(vertices))
{
}

Polygon::Polygon(Deserializer & deserializer) :
	m_vertices(std::move(deserializeVertices(deserializer)))
{
}

std::vector<Vec2f> PolygonNamespace::deserializeVertices(Deserializer & deserializer)
{
	deserializer.enterSegment(ms_segmentLabel);
	uint32 n = deserializer.getUint(ms_sizeLabel);
	std::vector<Vec2f> m_vertices;
	m_vertices.reserve(n);
	deserializer.enterSegment(ms_verticesLabel);
	for (uint32 i = 0; i < n; ++i)
	{
		deserializer.enterSegment(nullptr);
		m_vertices.emplace_back(deserializer);
		deserializer.leaveSegment();
	}
	deserializer.leaveSegment();
	deserializer.leaveSegment();
	return m_vertices;
}