#include "LuaParticles.h"

namespace LuaParticles
{
	static std::vector<Gear::ParticleSystem*>* g_particles = nullptr;
	//static int* g_nrOfParticles = nullptr;
	void registerFunctions(lua_State* lua, std::vector<Gear::ParticleSystem*>* particleSystem)
	{
		g_particles = particleSystem;
		//g_nrOfParticles = nrOfParticles;

		luaL_newmetatable(lua, "particleMeta");
		luaL_Reg regs[] =
		{
			{ "Bind"	   ,	bind },
			{ "SetPosition",	setPosition },
			{ "SetAlive",		setAlive },
			{ NULL, NULL }
		};
		luaL_setfuncs(lua, regs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");

		lua_setglobal(lua, "Particle");
	}

	int bind(lua_State* lua)
	{
		if (lua_gettop(lua) >= 2)
		{
			lua_pushinteger(lua, g_particles->size());
			g_particles->push_back(new Gear::ParticleSystem(lua_tointeger(lua, 1), lua_tonumber(lua, 2), lua_tonumber(lua, 3), lua_tonumber(lua, 4), lua_tointeger(lua, 5)));
		}
		return 1;
	}

	int setPosition(lua_State* lua)
	{
		if (lua_gettop(lua) >= 4)
		{
			int index = lua_tointeger(lua, 1);
			if (g_particles->at(index)->isActive)
			{
				glm::vec3 pos(lua_tonumber(lua, 2), lua_tonumber(lua, 3), lua_tonumber(lua, 4));
				g_particles->at(index)->setEmmiterPos(pos);
			}
		}
		return 0;
	}

	int setAlive(lua_State* lua)
	{
		if (lua_gettop(lua) >= 2)
		{
			int index = lua_tointeger(lua, 1);
			g_particles->at(index)->activate(lua_toboolean(lua, 2));
		}
		return 0;
	}
}