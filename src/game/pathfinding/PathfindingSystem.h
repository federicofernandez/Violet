#ifndef GAME_PathfindingSystem_H
#define GAME_PathfindingSystem_H

#include "engine/system/System.h"
#include "game/Config.h"
#include "game/pathfinding/Path.h"
#include "game/pathfinding/PathfindingComponent.h"

namespace Violet
{
	class Scene;
	class SystemFactory;
}

class VIOLET_GAME_API PathfindingSystem : public Violet::System
{
public:

	static const char * getStaticLabel();
	static void install(Violet::SystemFactory & factory);
	static std::unique_ptr<Violet::System> init(Violet::Deserializer & deserializer);

public:

	PathfindingSystem(PathfindingSystem && other);

	virtual ~PathfindingSystem() override;
	void update(float dt, Violet::Engine & engine) override;

	void path(Violet::Scene & scene, Violet::Entity entity, const Vec2f & goal);

private:

	PathfindingSystem();

	void createMap(Violet::Deserializer & deserializer, Violet::Engine & engine);
};

#endif
