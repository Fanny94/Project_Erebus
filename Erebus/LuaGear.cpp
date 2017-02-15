#include "LuaGear.h"
using namespace Gear;
using namespace Importer;

namespace LuaGear
{
	static GearEngine* g_gearEngine = nullptr;
	static std::vector<ModelInstance>* g_models = nullptr;
	static std::vector<ModelInstance>* g_animatedModels = nullptr;
	static std::vector<ModelInstance>* g_ForwardModels = nullptr;
	static std::vector<ModelInstance>* g_blendingModels = nullptr;
	//static std::vector<AnimatedInstance>* g_animatedModels = nullptr;
	static Animation* g_animations = nullptr;
	static int* g_boundAnimations = nullptr;
	static Assets* g_assets = nullptr;
	static WorkQueue* g_work = nullptr;
	static bool* g_queueModels = nullptr;
	static bool* g_mouseVisible = nullptr;
	static bool* g_fullscreen = nullptr;
	static TransformHandler* g_transformHandler = nullptr;

	void registerFunctions(lua_State* lua, GearEngine* gearEngine, std::vector<ModelInstance>* models, std::vector<ModelInstance>* animatedModels, Animation* animations, int* boundAnimations, std::vector<ModelInstance>* forwardModels, std::vector<ModelInstance>* blendingModels, TransformHandler* transformHandler, bool* queueModels, bool* mouseVisible, bool* fullscreen, Assets* assets, WorkQueue* work)
	{
		g_gearEngine = gearEngine;
		g_ForwardModels = forwardModels;
		g_blendingModels = blendingModels;
		g_models = models;
		g_animatedModels = animatedModels;
		g_animations = animations;
		g_boundAnimations = boundAnimations;
		g_assets = assets;
		g_work = work;
		g_queueModels = queueModels;
		g_mouseVisible = mouseVisible;
		g_fullscreen = fullscreen;
		g_transformHandler = transformHandler;

		// Gear
		luaL_newmetatable(lua, "gearMeta");
		luaL_Reg regs[] =
		{
			/*{ "AddStaticInstance", addStaticInstance },
			{ "AddAnimatedInstance", addAnimatedInstance },
			{ "AddForwardInstance",	addForwardInstance },
			{ "AddBlendingInstance", addBlendingInstance},*/
			{ "BindStaticInstance", bindStaticInstance },
			{ "BindAnimatedInstance", bindAnimatedInstance },
			{ "BindForwardInstance", bindForwardInstance },
			{ "BindBlendingInstance", bindBlendingInstance },
			{ "Print", print },
			{ "GetTextDimensions", getTextDimensions },
			{ "SetUniformValue", setUniformValue },
			{ "SetUniformLocation", setUniformLocation },
			{ "SetBlendUniformValue", setBlendUniformValue },
			{ "SetBlendTextures", setBlendTextures },
			{ "QueueModels", setQueueModels },
			{ "CursorVisible", setCursorVisible },
			{ "Fullscreen", setFullscreen },
			{ NULL, NULL }
		};

		luaL_setfuncs(lua, regs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");

		lua_newtable(lua);
		luaL_setmetatable(lua, "gearMeta");
		lua_pushlightuserdata(lua, gearEngine);
		lua_setfield(lua, -2, "__self");
		lua_setglobal(lua, "Gear");
		lua_pop(lua, 1);

		// Animation
		luaL_newmetatable(lua, "animationMeta");
		luaL_Reg animationRegs[] =
		{
			{ "Bind", bindAnimation },
			{ "Update",	updateAnimationBlending },
			{ "UpdateShaderMatrices", assembleAnimationsIntoShadermatrices },
			{ "SetTransitionTimes", setTransitionTimes },
			{ "SetAnimationSegments", setAnimationSegments },
			{ "QuickBlend", quickBlend },
			{ "SetSegmentState", setSegmentState },
			{ "SetQuickBlend", setQuickBlend },
			{ "SetAnimationPlayTime", setAnimationPlayTime },
			{ NULL, NULL }
		};

		luaL_setfuncs(lua, animationRegs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");
		lua_setglobal(lua, "Animation");
	}

	/*int addStaticInstance( lua_State* lua )
	{
		assert(lua_gettop(lua) == 2);

		ModelAsset* asset = (ModelAsset*)lua_touserdata(lua, 1);
		int transformID = (int)lua_tointeger(lua, 2);

		int result = g_gearEngine->generateWorldMatrix();

		int index = -1;
		for (int i = 0; i<g_models->size(); i++)
			if (g_models->at(i).asset == asset)
				index = i;

		if (index < 0)
		{
			ModelInstance instance;
			instance.asset = asset;

			index = (int)g_models->size();
			g_models->push_back(instance);

		}

		g_models->at(index).worldIndices.push_back(transformID);

		return 0;
	}

	int addAnimatedInstance(lua_State* lua)
	{
		assert(lua_gettop(lua) == 3);

		ModelAsset* asset = (ModelAsset*)lua_touserdata(lua, 1);
		int transformID = (int)lua_tointeger(lua, 2);
		lua_getfield(lua, 3, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);

		int result = g_gearEngine->generateWorldMatrix();

		int index = -1;
		for (int i = 0; i<g_animatedModels->size(); i++)
			if (g_animatedModels->at(i).asset == asset)
				index = i;

		if (index < 0)
		{
			AnimatedInstance instance;
			instance.asset = asset;

			index = (int)g_animatedModels->size();
			g_animatedModels->push_back(instance);
		}

		animation->setAsset(asset);

		g_animatedModels->at(index).worldIndices.push_back(transformID);
		g_animatedModels->at(index).animations.push_back(animation);

		return 0;
	}*/

	int bindStaticInstance( lua_State* lua )
	{
		assert( lua_gettop( lua ) == 1 );

		ModelAsset* asset = (ModelAsset*)lua_touserdata( lua, 1 );
		
		int result = g_transformHandler->bindStaticInstance( asset );
		lua_pushinteger( lua, result );

		return 1;
	}

	int bindAnimatedInstance( lua_State* lua )
	{
		assert( lua_gettop( lua ) == 2 );

		ModelAsset* asset = (ModelAsset*)lua_touserdata( lua, 1 );
		lua_getfield( lua, 2, "__self" );
		Animation* animation = (Animation*)lua_touserdata( lua, -1 );

		assert( asset );
		assert( animation );

		int result = g_transformHandler->bindAnimatedInstance( asset, animation );
		lua_pushinteger( lua, result );

		return 1;
	}

	int setQueueModels(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		*g_queueModels = lua_toboolean(lua, 1) != 0;
		return 0;
	}

	int setCursorVisible(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		*g_mouseVisible = lua_toboolean(lua, 1) != 0;
		return 0;
	}

	int setFullscreen(lua_State * lua)
	{
		assert(lua_gettop(lua) >= 1);
		*g_fullscreen = lua_toboolean(lua, 1) != 0;
		return 0;
	}

	/*int addForwardInstance(lua_State * lua)
	{
		assert(lua_gettop(lua) == 2);

		int index = -1;

		ModelAsset* asset = (ModelAsset*)lua_touserdata(lua, 1);
		int transformID = (int)lua_tointeger(lua, 2);
		int result = g_gearEngine->generateWorldMatrix();
		for (int i = 0; i<g_ForwardModels->size(); i++)
			if (g_ForwardModels->at(i).asset == asset)
				index = i;
		if (index < 0)
		{
			ModelInstance instance;
			instance.asset = asset;

			index = (int)g_ForwardModels->size();
			g_ForwardModels->push_back(instance);
			g_gearEngine->uniValues.push_back({ "NULL",{ 0, 0 } });
		}
		g_ForwardModels->at(index).worldIndices.push_back(transformID);
		lua_pushinteger(lua, index);

		return 1;
	}*/

	int bindForwardInstance(lua_State * lua)
	{
		assert( lua_gettop( lua ) == 1 );

		ModelAsset* asset = (ModelAsset*)lua_touserdata(lua, 1);
		int result = g_transformHandler->bindForwardInstance( asset );
		lua_pushinteger(lua, result );

		return 1;
	}

	int print(lua_State* lua)
	{
		int ntop = lua_gettop(lua);
		assert(ntop == 3 || ntop == 4 || ntop == 5);

		std::string s = lua_tostring(lua, 1);
		float x = (float)lua_tonumber(lua, 2);
		float y = (float)lua_tonumber(lua, 3);

		float scale = 1.0f;
		int type = -1;
		if (ntop >= 4)
			type = lua_type(lua, 4);

		glm::vec4 color(1.0f);
		if (type == LUA_TNUMBER)
		{
			scale = (float)lua_tonumber(lua, 4);

			if (ntop >= 5)
			{
				for (int i = 0; i<4; i++)
				{
					lua_rawgeti(lua, 5, i + 1);
					color[i] = (float)lua_tonumber(lua, -1);
				}
			}
		}
		else if (type == LUA_TTABLE)
		{
			for (int i = 0; i<4; i++)
			{
				lua_rawgeti(lua, 4, i + 1);
				color[i] = (float)lua_tonumber(lua, -1);
			}
		}

		g_gearEngine->print(s, x, y, scale, color);

		return 0;
	}

	int getTextDimensions(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);

		glm::vec2 dim = g_gearEngine->getTextDimensions(lua_tostring(lua, 1));
		lua_pushnumber(lua, dim.x);
		lua_pushnumber(lua, dim.y);
		return 2;
	}

	int bindAnimation(lua_State* lua)
	{
		int index = *g_boundAnimations;
		Animation* animation = &g_animations[index];
		animation->setMatrixIndex(index);
		*g_boundAnimations = index + 1;

		lua_newtable(lua);
		luaL_setmetatable(lua, "animationMeta");
		lua_pushlightuserdata(lua, animation);
		lua_setfield(lua, -2, "__self");

		return 1;
	}

	int quickBlend(lua_State * lua)
	{
		assert(lua_gettop(lua) == 6);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);
		float dt = (float)lua_tonumber(lua, 2);
		int originState = (int)lua_tointeger(lua, 3);
		int transitionState = (int)lua_tointeger(lua, 4);
		float blendTime = (float)lua_tonumber(lua, 5);
		int animationSegment = (int)lua_tointeger(lua, 6);

		bool res = animation->quickBlend(dt, originState,
			transitionState, blendTime, animationSegment);

		lua_pushboolean(lua, res);

		return 1;
	}

	int updateAnimationBlending(lua_State* lua)
	{
		assert(lua_gettop(lua) == 4);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);
		float dt = (float)lua_tonumber(lua, 2);
		int layer = (int)lua_tointeger(lua, 3);
		int animationSegment = (int)lua_tointeger(lua, 4);

		animation->updateState(dt, layer, animationSegment);

		return 1;
	}

	int setTransitionTimes(lua_State * lua)
	{
		assert(lua_gettop(lua) == 2);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);

		int numStates = (int)lua_rawlen(lua, 2);

		float* transitions = new float[numStates*numStates];

		for (int curState = 0, index = 0; curState < numStates; curState++)
		{
			lua_rawgeti(lua, 2, curState + 1);
			for (int curTransition = 0; curTransition < numStates; curTransition++, index++)
			{
				lua_rawgeti(lua, -1, curTransition + 1);
				transitions[index] = (float)lua_tonumber(lua, -1);
				lua_pop(lua, 1);
			}
			lua_pop(lua, 1);
		}

		animation->setTransitionTimes(transitions, numStates);

		delete[] transitions;

		return 0;
	}

	int setAnimationSegments(lua_State * lua)
	{
		assert(lua_gettop(lua) == 2);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);
		int numberOfSegments = (int)lua_tointeger(lua, 2);
		animation->setAnimationSegments(numberOfSegments);

		return 1;
	}

	int assembleAnimationsIntoShadermatrices(lua_State * lua)
	{
		assert(lua_gettop(lua) == 1);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);
		animation->assembleAnimationsIntoShadermatrices();

		return 0;
	}

	int setAnimationPlayTime(lua_State * lua)
	{

		assert(lua_gettop(lua) >= 3);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);

		float animTime = (float)lua_tonumber(lua, 2);
		int segment = (int)lua_tointeger(lua, 3);

		animation->setAnimationPlayTime(animTime, segment);

		return 0;
	}

	int setSegmentState(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 3);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);

		int state = (int)lua_tointeger(lua, 2);
		int segment = (int)lua_tointeger(lua, 3);

		animation->setSegmentState(state, segment);
		return 0;
	}

	int setQuickBlend(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 5);

		lua_getfield(lua, 1, "__self");
		Animation* animation = (Animation*)lua_touserdata(lua, -1);

		int from = (int)lua_tointeger(lua, 2);
		int to = (int)lua_tointeger(lua, 3);
		float blendTime = (float)lua_tointeger(lua, 4);
		int segment = (int)lua_tointeger(lua, 5);

		animation->setQuickBlend(from, to, blendTime, segment);
		return 0;
	}

	int setUniformValue(lua_State * lua)
	{
		assert(lua_gettop(lua) == 3);
		//g_gearEngine->uniValues.at((int)lua_tointeger(lua, 1)).values = { (float)lua_tonumber(lua, 2), (float)lua_tonumber(lua, 3) };

		int index = (int)lua_tointeger( lua, 1 );

		TransformHandle handle = g_transformHandler->getHandle( index );

		float a = (float)lua_tonumber( lua, 2 );
		float b = (float)lua_tonumber( lua, 3 );

		g_gearEngine->uniValues.at(handle.modelIndex).values = { a, b };

		return 0;
	}

	int setUniformLocation(lua_State* lua)
	{
		assert(lua_gettop(lua) == 2);
		//g_gearEngine->uniValues.at((int)lua_tointeger(lua, 1)).location = (std::string)lua_tostring(lua, 2);

		int index = (int)lua_tointeger( lua, 1 );

		TransformHandle handle = g_transformHandler->getHandle( index );

		std::string location = lua_tostring( lua, 2 );

		g_gearEngine->uniValues.at( handle.modelIndex ).location = location;

		return 0;
	}

	int setBlendUniformValue(lua_State * lua)
	{
		/*if (lua_gettop(lua) >= 4)
		{
			int index = (int)lua_tointeger(lua, 1);
			int size = (int)lua_tointeger(lua, 2);
			glm::vec2 blend;
			for (int i = 0; i < size; i++)
			{
				lua_getfield(lua, 3 + i, "x");
				blend.x = (float)lua_tonumber(lua, -1);

				lua_getfield(lua, 3 + i, "y");
				blend.y = (float)lua_tonumber(lua, -1);

				g_gearEngine->textureBlend.at(index).blendFactor[i] = blend;
			}

		}
		return 0;*/

		assert( lua_gettop( lua ) >= 4 );

		int index = (int)lua_tointeger(lua, 1);
		int size = (int)lua_tointeger(lua, 2);

		TransformHandle handle = g_transformHandler->getHandle( index );
		
		glm::vec2 blend;
		for (int i = 0; i < size; i++)
		{
			lua_getfield(lua, 3 + i, "x");
			blend.x = (float)lua_tonumber(lua, -1);

			lua_getfield(lua, 3 + i, "y");
			blend.y = (float)lua_tonumber(lua, -1);

			//g_gearEngine->textureBlend.at(index).blendFactor[i] = blend;
			g_gearEngine->textureBlend.at(handle.modelIndex).blendFactor[i] = blend;
		}

		return 0;
	}

	int setBlendTextures(lua_State * lua)
	{
		/*if (lua_gettop(lua) >= 4)
		{
			int index = (int)lua_tointeger(lua, 1);
			int size = (int)lua_tointeger(lua, 2);
			TextureAsset* texture;
			for (int i = 0; i < size; i++)
			{
				lua_getfield(lua, i + 3, "__self");
				texture = (TextureAsset*)lua_touserdata(lua, -1);

				g_gearEngine->textureBlend.at(index).textureVector.push_back(texture);
				g_gearEngine->textureBlend.at(index).numTextures = size;
			}
		}
		return 0;*/

		assert( lua_gettop( lua ) >= 4 );

		int index = (int)lua_tointeger( lua, 1 );
		int size = (int)lua_tointeger( lua, 2 );

		TransformHandle handle = g_transformHandler->getHandle( index );

		TextureAsset* texture;
		for( int i=0; i<size; i++ )
		{
			lua_getfield( lua, i+3, "__self" );
			texture = (TextureAsset*)lua_touserdata( lua, -1 );

			g_gearEngine->textureBlend.at( handle.modelIndex ).textureVector.push_back( texture );
			g_gearEngine->textureBlend.at( handle.modelIndex ).numTextures = size;
		}

		return 0;
	}

	/*int addBlendingInstance(lua_State * lua)
	{
		int ntop = lua_gettop(lua);
		int index = -1;
		if (ntop >= 2)
		{
			ModelAsset* asset = (ModelAsset*)lua_touserdata(lua, 1);
			int transformID = lua_tointeger(lua, 2);
			int result = g_gearEngine->generateWorldMatrix();
			for (int i = 0; i<g_blendingModels->size(); i++)
				if (g_blendingModels->at(i).asset == asset)
					index = i;
			if (index < 0)
			{
				ModelInstance instance;
				instance.asset = asset;

				TextureBlendings tBlend;
				g_gearEngine->textureBlend.push_back(tBlend);
				index = g_blendingModels->size();
				g_blendingModels->push_back(instance);
			}
			g_blendingModels->at(index).worldIndices.push_back(transformID);
			g_gearEngine->textureBlend.at(index).modelIndex = index;
		}
		lua_pushinteger(lua, index);
		return 1;
	}*/
	int bindBlendingInstance(lua_State * lua)
	{
		assert( lua_gettop( lua ) == 1 );

		ModelAsset* asset = (ModelAsset*)lua_touserdata(lua, 1);
		int result = g_transformHandler->bindBlendingInstance( asset );

		lua_pushinteger(lua, result );
		return 1;
	}
}
