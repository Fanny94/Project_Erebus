#include "LuaSound.h"

namespace LuaSound
{
	static SoundEngine* g_soundEngine;

	void registerFunctions(lua_State* lua, SoundEngine* soundEngine)
	{
		g_soundEngine = soundEngine;

		luaL_newmetatable(lua, "soundMeta");
		luaL_Reg regs[] =
		{
			{ "Play", play },
			{ "PauseAll", pauseAll },
			{ "ResumeAll", resumeAll },
			{ "SetMasterVolume", setMasterVolume },
			{ "SetPlayerTransform", setPlayerTransform },
			{ NULL, NULL }
		};

		luaL_setfuncs(lua, regs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");
		lua_setglobal(lua, "Sound");
	}

	int play(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		std::string s = lua_tostring(lua, 1);
		g_soundEngine->play(s);
		return 0;
	}

	int play3D(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		std::string s = lua_tostring(lua, 1);
		glm::vec3 pos(0.0f);
		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 2, i + 1);
			pos[i] = lua_tonumber(lua, -1);
		}
		g_soundEngine->play(s, SOUND_3D, pos);
		return 0;
	}

	int pauseAll(lua_State* lua)
	{
		g_soundEngine->pauseAll();
		return 0;
	}

	int resumeAll(lua_State* lua)
	{
		g_soundEngine->resumeAll();
		return 0;
	}

	int setMasterVolume(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		float v = lua_tonumber(lua, 1);
		g_soundEngine->setMasterVolume(v);
		return 0;
	}

	int setPlayerTransform(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		glm::vec3 pos(0.0f);
		glm::vec3 look(0.0f);

		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 1, i + 1);
			pos[i] = lua_tonumber(lua, -1);
		}

		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 2, i + 1);
			look[i] = lua_tonumber(lua, -1);
		}
		g_soundEngine->setPlayerTransform(pos, look);
		return 0;
	}
}