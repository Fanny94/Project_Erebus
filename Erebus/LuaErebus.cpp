#include "LuaErebus.h"

namespace LuaErebus
{
	static Transform* g_transforms = nullptr;
	static Controls* g_controls = nullptr;
	static NetworkController* g_network = nullptr;
	static PerformanceCounter* g_counter = nullptr;

	void registerFunctions( lua_State* lua, Transform* transforms, Controls* controls, NetworkController* network, PerformanceCounter* counter )
	{
		g_transforms = transforms;
		g_controls = controls;
		g_network = network;
		g_counter = counter;

		luaL_newmetatable( lua, "erebusMeta" );
		luaL_Reg regs[] =
		{
			{ "SetControls", setControls },
			{ "CameraSensitivity", cameraSensitivity },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, regs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "Erebus" );
	}

	int setControls( lua_State* lua )
	{
		if( lua_gettop( lua ) >= 1 )
		{
			int transformID = lua_tointeger( lua, 1 );
			g_controls->setControl( &g_transforms[transformID] );
		}

		return 0;
	}
	
	int cameraSensitivity(lua_State * lua)
	{		
		g_controls->sensitivityFactor(lua_tonumber(lua, 1));
		return 0;
	}

	int startNetwork( lua_State* lua )
	{
		assert( lua_gettop( lua ) >= 1 );

		bool host = lua_toboolean( lua, 1 );
		g_network->setNetworkHost( host );
		if( host )
		{
			if( g_network->initNetworkAsHost() )
				g_network->acceptNetworkCommunication();
		}
		else
		{
			g_network->initNetworkAsClient(127,0,0,1);
		}

		g_network->startCommunicationThreads(*g_counter);
		return 0;
	}
}