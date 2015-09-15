#ifndef GAME_MapComponent_H
#define GAME_MapComponent_H

#include "violet/component/Component.h"
#include "violet/entity/Entity.h"
#include "game/Config.h"
#include "game/pathfinding/Map.h"

namespace Violet
{
	class Deserializer;
	class Engine;
}

class VIOLET_GAME_API MapComponent : public Violet::ComponentBase<MapComponent>
{
public:

	static Violet::Tag getStaticTag();

public:

	MapComponent(Violet::Entity & owner, Violet::Deserializer & deserializer);
	MapComponent(MapComponent && other);

public:

	Map m_graph;
	uint32 m_id;
};

#endif