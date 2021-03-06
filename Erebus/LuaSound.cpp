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
			{ "Fade", fade },
			{ "Crossfade", crossfade },
			{ "Play", play },
			{ "Pause", pause},
			{ "Resume", resume},
			{ "Stop", stop },
			{ "PauseAll", pauseAll },
			{ "ResumeAll", resumeAll },
			{ "ClearAll", clearAll },
			{ "SetMasterVolume", setMasterVolume },
			{ "SetVolume", setVolume},
			{ "SetPosition", setPosition},
			{ "SetVelocity", setVelocity},
			{ "SetPlayerTransform", setPlayerTransform },
			{ NULL, NULL }
		};

		luaL_setfuncs(lua, regs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");
		lua_setglobal(lua, "Sound");
	}

	int fade(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		int index = (int)lua_tointeger(lua, 1);
		float time = (float)lua_tonumber(lua, 2);
		g_soundEngine->fade(index, time);
		return 0;
	}

	int crossfade(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 3);
		int from = (int)lua_tointeger(lua, 1);
		int to = (int)lua_tointeger(lua, 2);
		float time = (float)lua_tonumber(lua, 3);
		g_soundEngine->crossfade(from, to, time);
		return 0;
	}

	int play(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		std::string s = lua_tostring(lua, 1);
		int options = SOUND_NO_FLAG;
		glm::vec3 pos(0.f);

		if (lua_gettop(lua) >= 2)
			options = (int)lua_tointeger(lua, 2);
		if (lua_gettop(lua) >= 3)
		{
			for (int i = 0; i < 3; i++)
			{
				lua_rawgeti(lua, 3, i + 1);
				pos[i] = (float)lua_tonumber(lua, -1);
			}
		}

		int index = (int)g_soundEngine->play(s, options, pos);
		lua_pushinteger(lua, index);
		return 1;
	}

	int pause(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		int index = (int)lua_tointeger(lua, 1);
		g_soundEngine->pause(index);
		return 0;
	}

	int resume(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		int index = (int)lua_tointeger(lua, 1);
		g_soundEngine->resume(index);
		return 0;
	}

	int stop(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		int index = (int)lua_tointeger(lua, 1);
		g_soundEngine->stop(index);
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

	int clearAll(lua_State* lua)
	{
		g_soundEngine->clearAll();
		return 0;
	}

	int setMasterVolume(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 1);
		float v = (float)lua_tonumber(lua, 1);
		g_soundEngine->setMasterVolume(v);
		return 0;
	}

	int setVolume(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		int i = (int)lua_tointeger(lua, 1);
		float v = (float)lua_tonumber(lua, 2);
		g_soundEngine->setVolume(i, v);
		return 0;
	}

	int setPosition(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		int i = (int)lua_tointeger(lua, 1);
		glm::vec3 pos(0.f);
		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 2, i + 1);
			pos[i] = (float)lua_tonumber(lua, -1);
		}
		g_soundEngine->setPosition(i, pos);
		return 0;
	}

	int setVelocity(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		int i = (int)lua_tointeger(lua, 1);
		glm::vec3 vel(0.f);
		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 2, i + 1);
			vel[i] = (float)lua_tonumber(lua, -1);
		}
		g_soundEngine->setPosition(i, vel);
		return 0;
	}

	int setPlayerTransform(lua_State* lua)
	{
		assert(lua_gettop(lua) >= 2);
		glm::vec3 pos(0.f);
		glm::vec3 look(0.f);

		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 1, i + 1);
			pos[i] = (float)lua_tonumber(lua, -1);
		}

		for (int i = 0; i < 3; i++)
		{
			lua_rawgeti(lua, 2, i + 1);
			look[i] = (float)lua_tonumber(lua, -1);
		}
		g_soundEngine->setPlayerTransform(pos, look);
		return 0;
	}
}