#pragma once
#include <lua\lua.hpp>
#include "BaseIncludes.h"
#include "Controls.h"
#include "NetworkController.hpp"

#include "LuaErebus.h"
#include "LuaAssets.h"
#include "LuaCollision.h"
#include "LuaGear.h"
#include "LuaParticles.h"
#include "LuaTransform.h"
#include "LuaCamera.h"
#include "LuaAi.h"
#include "LuaInputs.h"
#include "LuaNetwork.hpp"
#include "WorkQueue.h"
#include "LuaSound.h"
#include "LuaUI.h"
#include "LuaLight.h"
#include "LuaMath.h"

class LuaBinds
{
public:
	LuaBinds();
	~LuaBinds();

	void load( Gear::GearEngine* gearEngine,
				Importer::Assets* assets,
				CollisionHandler* collisionHandler,
				CollisionsDraw* collisionsDraw,
				Controls* controls,
				Inputs* inputs,
				Animation* animations,
				int* boundAnimations,
				std::vector<ModelInstance>* models,
				//std::vector<AnimatedInstance>* animatedModels,
				std::vector<ModelInstance>* animatedModels,
				std::vector<ModelInstance>* forwardModels,
				std::vector<ModelInstance>* blendingModels,
				TransformHandler* transformHandler,
				bool* queueModels,
				bool* mouseVisible,
				bool* fullscreen,
				bool* running,
				Camera* camera,
				std::vector<Gear::ParticleSystem*>* ps,
				std::vector<Gear::ParticleEmitter*>* emitters,
				AGI::AGIEngine* AI,
				NetworkController* network,
				WorkQueue* work,
				SoundEngine* soundEngine,
				PerformanceCounter* counter,
				FloatingDamage* floatingDamage);
	void unload();
	void update( Controls* controls, float deltaTime );
	void printLuaTop() const;
	lua_State* getState();

private:
	lua_State* lua;
	bool validState;
	int updateReference;
};