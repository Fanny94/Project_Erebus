#pragma once

#include "lua\lua.hpp"
#include "NetworkController.hpp"

namespace LuaNetwork
{
	void registerFunctions(lua_State* lua, NetworkController* networkController);
	int sendTransformPacket(lua_State* lua);
	int getTransformPacket(lua_State* lua);
	int sendAnimationPacket(lua_State* lua);
	int getAnimationPacket(lua_State* lua);
	int sendAIStatePacket(lua_State* lua);
	int getAIStatePacket(lua_State* lua);
	int sendSpellPacket(lua_State* lua);
	int getSpellPacket(lua_State* lua);
	int sendAITransformPacket(lua_State* lua);
	int getAITransformPacket(lua_State* lua);
	int sendChargingPacket(lua_State* lua);
	int getChargingPacket(lua_State* lua);
	int getNetworkHost(lua_State* lua);
	int shouldSendNewAITransform(lua_State* lua);
	int shouldSendNewTransform(lua_State* lua);
	int shouldSendNewAnimation(lua_State* lua);
}
