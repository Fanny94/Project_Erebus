#pragma once

#include "lua\lua.hpp"
#include "Gear.h"
#include "Assets.h"
#include "WorkQueue.h"

namespace LuaGear
{
	void registerFunctions( lua_State* lua,
							Gear::GearEngine* gearEngine,
							std::vector<ModelInstance>* models,
							std::vector<AnimatedInstance>* animatedModels,
							Assets* assets,
							WorkQueue* work );

	int addStaticInstance( lua_State* lua );
	int addAnimatedInstance( lua_State* lua );
	int activateInstance( lua_State* lua );
	int deactivateInstance( lua_State* lua );

	int print( lua_State* lua );
	int getTextDimensions( lua_State* lua );

	int createAnimation( lua_State* lua );
	int destroyAnimation( lua_State* lua );
	int updateAnimation( lua_State* lua );

	void resetAnimations();
	void asyncAnimation( void* args );

	struct AnimationData
	{
		Animation* animation;
		float dt;
		int layer;
	};
}