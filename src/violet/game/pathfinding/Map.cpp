#include "violet/game/pathfinding/Map.h"

#include "violet/core/Engine.h"
#include "violet/core/math/Polygon.h"
#include "violet/core/serialization/Deserializer.h"

uint32 Map::Road::getSource() const
{
	return m_src;
}

Map::Map() :
	m_graph(),
	m_roadWidth(0)
{
}

Map::Map(Violet::Deserializer & deserializer) :
	Map()
{
	auto const & mapSegment = deserializer.enterSegment("map");
	m_roadWidth = mapSegment->getFloat("roadWidth");
	{
		auto nodesSegment = mapSegment->enterSegment("nodes");
		const uint32 n = nodesSegment->getUint("n");
		for (uint32 i = 0; i < n; ++i)
			m_graph.addNode({ Vec2f(*nodesSegment) });
	}

	{
		auto edgesSegment = mapSegment->enterSegment("edges");
		const uint32 n = edgesSegment->getUint("n");
		for (uint32 i = 0; i < n; ++i)
		{
			const uint32 src = edgesSegment->getUint("src");
			const uint32 dst = edgesSegment->getUint("dst");
			m_graph.addEdge({ src, dst });
		}
	}
}

Violet::CrsGraph<Map::Intersection, Map::Road> & Map::getGraph()
{
	return m_graph;
}

const Violet::CrsGraph<Map::Intersection, Map::Road> & Map::getGraph() const
{
	return m_graph;
}