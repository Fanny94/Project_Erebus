#pragma once
#include <lua\lua.hpp>
#include "BaseIncludes.h"
#include "Controls.h"

#include "LuaErebus.h"
#include "LuaAssets.h"
#include "LuaCollision.h"
#include "LuaGear.h"
#include "LuaParticles.h"
#include "LuaTransform.h"
#include "LuaCamera.h"
#include"LuaAi.h"
#include "LuaInputs.h"
#include "WorkQueue.h"

class LuaBinds
{
public:
	LuaBinds();
	~LuaBinds();

	void load( Gear::GearEngine* gearEngine,
				Importer::Assets* assets,
				CollisionHandler* collisionHandler,
				Controls* controls,
				Inputs* inputs,
				Transform* transforms,
				int* boundTransforms,
				std::vector<ModelInstance>* models,
				std::vector<AnimatedInstance>* animatedModels,
				Camera* camera,
				std::vector<Gear::ParticleSystem*>* ps,
				AGI::AGIEngine* AI,
				WorkQueue* work );
	void unload();
	void update( Controls* controls, float deltaTime );
	void printLuaTop() const;


private:
	lua_State* lua;
	bool validState;
	int updateReference;
};