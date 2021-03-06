#include "LuaNetwork.hpp"

namespace LuaNetwork 
{
	static NetworkController* g_networkController = nullptr;

	void registerFunctions(lua_State * lua, NetworkController * networkController)
	{
		g_networkController = networkController;
		luaL_newmetatable(lua, "networkMeta");
		luaL_Reg regs[] =
		{
			{ "SendTransformPacket", sendTransformPacket },
			{ "GetTransformPacket", getTransformPacket },
			{ "SendAnimationPacket", sendAnimationPacket },
			{ "GetAnimationPacket", getAnimationPacket },
			{ "SendAIStatePacket", sendAIStatePacket },
			{ "GetAIStatePacket", getAIStatePacket },
			{ "SendSpellPacket", sendSpellPacket },
			{ "SendChargeSpellPacket", sendChargeSpellPacket },
			{ "GetSpellPacket", getSpellPacket },
			{ "SendAITransformPacket", sendAITransformPacket },
			{ "GetAITransformPacket", getAITransformPacket },
			{ "SendChargingPacket", sendChargingPacket },
			{ "GetChargingPacket", getChargingPacket },
			{ "SendQuickBlendPacket", sendQuickBlendPacket },
			{ "GetQuickBlendPacket", getQuickBlendPacket },
			{ "SendDamagePacket", sendDamagePacket },
			{ "GetDamagePacket", getDamagePacket },
			{ "SendChangeSpellsPacket", sendChangeSpellsPacket },
			{ "GetChangeSpellsPacket", getChangeSpellsPacket },
			{ "SendPlayerEventPacket", sendPlayerEventPacket },
			{ "GetPlayerEventPacket", getPlayerEventPacket },
			{ "SendAIHealthPacket", sendAIHealthPacket },
			{ "GetAIHealthPacket", getAIHealthPacket },
			{ "SendDashPacket", sendDashPacket },
			{ "GetDashPacket", getDashPacket },
			{ "SendEndEventPacket", sendEndEventPacket },
			{ "GetEndEventPacket", getEndEventPacket },
			{ "SendPlayerHealthPacket", sendPlayerHealthPacket },
			{ "GetPlayerHealthPacket", getPlayerHealthPacket },
			{ "SendRessurectionPacket", sendRessurectionPacket },
			{ "SendAIDamageTextPacket", sendAIDamageTextPacket },
			{ "GetAIDamageTextPacket", getAIDamageTextPacket },
			{ "SendBossDamageTextPacket", sendBossDamageTextPacket },
			{ "GetBossDamageTextPacket", getBossDamageTextPacket },
			{ "SendBossHealthPacket", sendBossHealthPacket },
			{ "GetBossHealthPacket", getBossHealthPacket },
			{ "GetRessurectionPacket", getRessurectionPacket },
			{ "GetNetworkHost", getNetworkHost },
			{ "ShouldSendNewTransform", shouldSendNewTransform },
			{ "ShouldSendNewAnimation", shouldSendNewAnimation },
			{ "ShouldSendNewAITransform", shouldSendNewAITransform },
			{ "GetIP", getIP },
			{ NULL, NULL }
		};

		luaL_setfuncs(lua, regs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");

		lua_setglobal(lua, "Network");
	}

	int sendTransformPacket(lua_State* lua)
	{
		int index = (int)lua_tonumber(lua, 1);

		lua_getfield(lua, 2, "x");
		float pos_x = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 2, "y");
		float pos_y = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 2, "z");
		float pos_z = (float)lua_tonumber(lua, -1);

		lua_getfield(lua, 3, "x");
		float lookAt_x = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 3, "y");
		float lookAt_y = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 3, "z");
		float lookAt_z = (float)lua_tonumber(lua, -1);

		lua_getfield(lua, 4, "x");
		float rotation_x = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 4, "y");
		float rotation_y = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 4, "z");
		float rotation_z = (float)lua_tonumber(lua, -1);

		g_networkController->sendTransformPacket(TransformPacket(index, pos_x, pos_y, pos_z, lookAt_x, lookAt_y, lookAt_z, rotation_x, rotation_y, rotation_z));

		return 0;
	}

	int getTransformPacket(lua_State* lua)
	{
		TransformPacket transformPacket;

		if (g_networkController->fetchTransformPacket(transformPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, transformPacket.data.id);
			lua_pushnumber(lua, transformPacket.data.pos_x);
			lua_pushnumber(lua, transformPacket.data.pos_y);
			lua_pushnumber(lua, transformPacket.data.pos_z);
			lua_pushnumber(lua, transformPacket.data.lookAt_x);
			lua_pushnumber(lua, transformPacket.data.lookAt_y);
			lua_pushnumber(lua, transformPacket.data.lookAt_z);
			lua_pushnumber(lua, transformPacket.data.rotation_x);
			lua_pushnumber(lua, transformPacket.data.rotation_y);
			lua_pushnumber(lua, transformPacket.data.rotation_z);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 80);
			lua_pushnumber(lua, 27);
			lua_pushnumber(lua, 160);
			lua_pushnumber(lua, 0.5);
			lua_pushnumber(lua, 0.25);
			lua_pushnumber(lua, 0.75);
			lua_pushnumber(lua, 0.0);
			lua_pushnumber(lua, 3.14);
			lua_pushnumber(lua, 1.57);
		}

		return 11;
	}

	int sendAnimationPacket(lua_State* lua)
	{
		int animationState1 = (int)lua_tonumber(lua, 1);
		int animationState2 = (int)lua_tonumber(lua, 2);

		g_networkController->sendAnimationPacket(AnimationPacket(animationState1, animationState2));

		return 0;
	}

	int getAnimationPacket(lua_State* lua)
	{
		AnimationPacket animationPacket;

		if (g_networkController->fetchAnimationPacket(animationPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, animationPacket.data.animationState1);
			lua_pushnumber(lua, animationPacket.data.animationState2);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}
		
		return 3;
	}

	int sendAIStatePacket(lua_State* lua)
	{
		int index = (int)lua_tonumber(lua, 1);
		int aiState = (int)lua_tonumber(lua, 2);

		g_networkController->sendAIStatePacket(AIStatePacket(index, aiState));

		return 0;
	}

	int getAIStatePacket(lua_State* lua)
	{
		AIStatePacket aiPacket;

		if (g_networkController->fetchAIStatePacket(aiPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, aiPacket.data.transformID);
			lua_pushnumber(lua, aiPacket.data.aiState);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 3;
	}

	int sendSpellPacket(lua_State* lua)
	{
		int index = (int)lua_tonumber(lua, 1);
		int currentSpell = (int)lua_tonumber(lua, 2);
		float dirX = (float)lua_tonumber(lua, 3);
		float dirY = (float)lua_tonumber(lua, 4);
		float dirZ = (float)lua_tonumber(lua, 5);

		g_networkController->sendSpellPacket(SpellPacket(index, currentSpell, false, true, dirX, dirY, dirZ));

		return 0;
	}

	int sendChargeSpellPacket(lua_State* lua)
	{
		int index = (int)lua_tonumber(lua, 1);
		int currentSpell = (int)lua_tonumber(lua, 2);
		bool cast = lua_toboolean(lua, 3) != 0;
		float dirX = (float)lua_tonumber(lua, 4);
		float dirY = (float)lua_tonumber(lua, 5);
		float dirZ = (float)lua_tonumber(lua, 6);

		g_networkController->sendSpellPacket(SpellPacket(index, currentSpell, true, cast, dirX, dirY, dirZ));

		return 0;
	}

	int getSpellPacket(lua_State* lua)
	{
		SpellPacket spellPacket;

		if (g_networkController->fetchSpellPacket(spellPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, spellPacket.data.ID);
			lua_pushnumber(lua, spellPacket.data.currentSpell);
			lua_pushboolean(lua, spellPacket.data.chargeSpell);
			lua_pushboolean(lua, spellPacket.data.cast);
			lua_pushnumber(lua, spellPacket.data.dirX);
			lua_pushnumber(lua, spellPacket.data.dirY);
			lua_pushnumber(lua, spellPacket.data.dirZ);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushboolean(lua, false);
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 8;
	}

	int sendAITransformPacket(lua_State* lua)
	{
		int index = (int)lua_tonumber(lua, 1);

		lua_getfield(lua, 2, "x");
		float pos_x = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 2, "y");
		float pos_y = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 2, "z");
		float pos_z = (float)lua_tonumber(lua, -1);

		lua_getfield(lua, 3, "x");
		float lookAt_x = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 3, "y");
		float lookAt_y = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 3, "z");
		float lookAt_z = (float)lua_tonumber(lua, -1);

		lua_getfield(lua, 4, "x");
		float rotation_x = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 4, "y");
		float rotation_y = (float)lua_tonumber(lua, -1);
		lua_getfield(lua, 4, "z");
		float rotation_z = (float)lua_tonumber(lua, -1);

		g_networkController->sendAITransformPacket(TransformPacket(index, pos_x, pos_y, pos_z, lookAt_x, lookAt_y, lookAt_z, rotation_x, rotation_y, rotation_z));

		return 0;
	}

	int getAITransformPacket(lua_State* lua)
	{
		TransformPacket aiTransformPacket;

		if (g_networkController->fetchAITransformPacket(aiTransformPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, aiTransformPacket.data.id);
			lua_pushnumber(lua, aiTransformPacket.data.pos_x);
			lua_pushnumber(lua, aiTransformPacket.data.pos_y);
			lua_pushnumber(lua, aiTransformPacket.data.pos_z);
			lua_pushnumber(lua, aiTransformPacket.data.lookAt_x);
			lua_pushnumber(lua, aiTransformPacket.data.lookAt_y);
			lua_pushnumber(lua, aiTransformPacket.data.lookAt_z);
			lua_pushnumber(lua, aiTransformPacket.data.rotation_x);
			lua_pushnumber(lua, aiTransformPacket.data.rotation_y);
			lua_pushnumber(lua, aiTransformPacket.data.rotation_z);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 80);
			lua_pushnumber(lua, 27);
			lua_pushnumber(lua, 160);
			lua_pushnumber(lua, 0.5);
			lua_pushnumber(lua, 0.25);
			lua_pushnumber(lua, 0.75);
			lua_pushnumber(lua, 0.0);
			lua_pushnumber(lua, 3.14);
			lua_pushnumber(lua, 1.57);
		}

		return 11;
	}

	int sendChargingPacket(lua_State* lua)
	{
		uint16_t chosenSpell = (uint16_t)lua_tonumber(lua, 1);
		bool activateCombineRay = (bool)lua_toboolean(lua, 2);

		g_networkController->sendChargingPacket(ChargingPacket(chosenSpell, activateCombineRay));

		return 0;
	}

	int getChargingPacket(lua_State* lua)
	{
		ChargingPacket chargingPacket;

		if (g_networkController->fetchChargingPacket(chargingPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, chargingPacket.data.chosenSpell);
			lua_pushboolean(lua, chargingPacket.data.activateCombineRay);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushboolean(lua, false);

		}

		return 3;
	}

	int sendQuickBlendPacket(lua_State* lua)
	{
		uint16_t quickBlendFrom = (uint16_t)lua_tonumber(lua, 1);
		uint16_t quickBlendTo = (uint16_t)lua_tonumber(lua, 2);
		uint16_t damagedMaxTime = (uint16_t)lua_tonumber(lua, 3);
		uint16_t quickBlendSegment = (uint16_t)lua_tonumber(lua, 4);

		g_networkController->sendQuickBlendPacket(QuickBlendPacket(quickBlendFrom, quickBlendTo, damagedMaxTime, quickBlendSegment));

		return 0;
	}

	int getQuickBlendPacket(lua_State* lua)
	{
		QuickBlendPacket quickBlendPacket;

		if (g_networkController->fetchQuickBlendPacket(quickBlendPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, quickBlendPacket.data.quickBlendFrom);
			lua_pushnumber(lua, quickBlendPacket.data.quickBlendTo);
			lua_pushnumber(lua, quickBlendPacket.data.damagedMaxTime);
			lua_pushnumber(lua, quickBlendPacket.data.quickBlendSegment);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 5;
	}

	int sendDamagePacket(lua_State* lua)
	{
		uint16_t index = (uint16_t)lua_tonumber(lua, 1);
		float damage = (float)lua_tonumber(lua, 2);
		uint8_t element = (uint8_t)lua_tonumber(lua, 3);

		g_networkController->sendDamagePacket(DamagePacket(index, damage, element));

		return 0;
	}

	int getDamagePacket(lua_State* lua)
	{
		DamagePacket damagePacket;

		if (g_networkController->fetchDamagePacket(damagePacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, damagePacket.data.transformID);
			lua_pushnumber(lua, damagePacket.data.damage);
			lua_pushnumber(lua, damagePacket.data.element);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 4;
	}

	int sendChangeSpellsPacket(lua_State* lua)
	{
		uint8_t spellSlot1 = (uint8_t)lua_tonumber(lua, 1);
		uint8_t spellSlot2 = (uint8_t)lua_tonumber(lua, 2);
		uint8_t spellSlot3 = (uint8_t)lua_tonumber(lua, 3);

		g_networkController->sendChangeSpellsPacket(ChangeSpellsPacket(spellSlot1, spellSlot2, spellSlot3));

		return 0;
	}

	int getChangeSpellsPacket(lua_State* lua)
	{
		ChangeSpellsPacket changeSpellsPacket;

		if (g_networkController->fetchChangeSpellsPacket(changeSpellsPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, changeSpellsPacket.data.spellSlot1);
			lua_pushnumber(lua, changeSpellsPacket.data.spellSlot2);
			lua_pushnumber(lua, changeSpellsPacket.data.spellSlot3);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 4;
	}

	int sendPlayerEventPacket(lua_State* lua)
	{
		uint8_t eventId = (uint8_t)lua_tonumber(lua, 1);

		g_networkController->sendPlayerEventPacket(EventPacket(eventId));
		
		return 0;
	}

	int getPlayerEventPacket(lua_State* lua)
	{
		EventPacket playerEventPacket;

		if (g_networkController->fetchPlayerEventPacket(playerEventPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, playerEventPacket.data.eventID);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
		}
		return 2;
	}

	int sendAIHealthPacket(lua_State* lua)
	{
		uint16_t transformID = (uint16_t)lua_tonumber(lua, 1);
		uint16_t health = (uint16_t)lua_tonumber(lua, 2);

		g_networkController->sendAIHealthPacket(HealthPacket(transformID, health));

		return 0;
	}

	int getAIHealthPacket(lua_State* lua)
	{
		HealthPacket aiHealthPacket;

		if (g_networkController->fetchAIHealthPacket(aiHealthPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, aiHealthPacket.data.transformID);
			lua_pushnumber(lua, aiHealthPacket.data.health);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 3;
	}

	int sendDashPacket(lua_State* lua)
	{
		bool isDashing = (bool)lua_toboolean(lua, 1);

		g_networkController->sendDashPacket(DashPacket(isDashing));

		return 0;
	}

	int getDashPacket(lua_State* lua)
	{
		DashPacket dashPacket;

		if (g_networkController->fetchDashPacket(dashPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushboolean(lua, dashPacket.data.isDashing);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushboolean(lua, false);
		}

		return 2;
	}

	int sendEndEventPacket(lua_State* lua)
	{
		uint8_t eventId = (uint8_t)lua_tonumber(lua, 1);

		g_networkController->sendEndEventPacket(EventPacket(eventId));

		return 0;
	}

	int getEndEventPacket(lua_State* lua)
	{
		EventPacket endEventPacket;

		if (g_networkController->fetchEndEventPacket(endEventPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, endEventPacket.data.eventID);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
		}
		return 2;
	}

	int sendPlayerHealthPacket(lua_State* lua)
	{
		uint16_t transformID = (uint16_t)lua_tonumber(lua, 1);
		uint16_t health = (uint16_t)lua_tonumber(lua, 2);

		g_networkController->sendPlayerHealthPacket(HealthPacket(transformID, health));

		return 0;
	}

	int getPlayerHealthPacket(lua_State* lua)
	{
		HealthPacket playerHealthPacket;

		if (g_networkController->fetchPlayerHealthPacket(playerHealthPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, playerHealthPacket.data.transformID);
			lua_pushnumber(lua, playerHealthPacket.data.health);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 3;
	}

	int sendRessurectionPacket(lua_State* lua)
	{
		uint16_t transformID = (uint16_t)lua_tonumber(lua, 1);
		uint16_t health = (uint16_t)lua_tonumber(lua, 2);

		g_networkController->sendRessurectionPacket(HealthPacket(transformID, health));

		return 0;
	}

	int getRessurectionPacket(lua_State* lua)
	{
		HealthPacket ressurectionPacket;

		if (g_networkController->fetchRessurectionPacket(ressurectionPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, ressurectionPacket.data.transformID);
			lua_pushnumber(lua, ressurectionPacket.data.health);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 3;
	}

	int sendAIDamageTextPacket(lua_State* lua)
	{
		uint16_t index = (uint16_t)lua_tonumber(lua, 1);
		float damage = (float)lua_tonumber(lua, 2);
		uint8_t element = (uint8_t)lua_tonumber(lua, 3);

		g_networkController->sendAIDamageTextPacket(DamagePacket(index, damage, element));

		return 0;
	}

	int getAIDamageTextPacket(lua_State* lua)
	{
		DamagePacket damagePacket;

		if (g_networkController->fetchAIDamageTextPacket(damagePacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, damagePacket.data.transformID);
			lua_pushnumber(lua, damagePacket.data.damage);
			lua_pushnumber(lua, damagePacket.data.element);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 4;
	}

	int sendBossDamageTextPacket(lua_State* lua)
	{
		uint16_t index = (uint16_t)lua_tonumber(lua, 1);
		float damage = (float)lua_tonumber(lua, 2);
		uint8_t element = (uint8_t)lua_tonumber(lua, 3);

		g_networkController->sendBossDamageTextPacket(DamagePacket(index, damage, element));

		return 0;
	}

	int getBossDamageTextPacket(lua_State* lua)
	{
		DamagePacket damagePacket;

		if (g_networkController->fetchBossDamageTextPacket(damagePacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, damagePacket.data.transformID);
			lua_pushnumber(lua, damagePacket.data.damage);
			lua_pushnumber(lua, damagePacket.data.element);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 4;
	}

	int sendBossHealthPacket(lua_State* lua)
	{
		uint16_t transformID = (uint16_t)lua_tonumber(lua, 1);
		uint16_t health = (uint16_t)lua_tonumber(lua, 2);

		//uint16_t transformID = (uint16_t)lua_tonumber(lua, 1);
		//uint16_t health = (uint16_t)lua_tonumber(lua, 2);

		g_networkController->sendBossHealthPacket(HealthPacket(transformID, health));

		return 0;
	}

	int getBossHealthPacket(lua_State* lua)
	{
		HealthPacket bossHealthPacket;

		if (g_networkController->fetchBossHealthPacket(bossHealthPacket))
		{
			lua_pushboolean(lua, true);
			lua_pushnumber(lua, bossHealthPacket.data.transformID);
			lua_pushnumber(lua, bossHealthPacket.data.health);
		}
		else
		{
			lua_pushboolean(lua, false);
			lua_pushnumber(lua, 0);
			lua_pushnumber(lua, 0);
		}

		return 3;
	}

	int getNetworkHost(lua_State* lua)
	{
		bool networkHost = g_networkController->getNetworkHost();

		lua_pushboolean(lua, networkHost);
		return 1;
	}

	int shouldSendNewTransform(lua_State* lua)
	{
		if (g_networkController->timeSinceLastTransformPacket() > 0.0167)
		{
			lua_pushboolean(lua, true);
		}
		else
		{
			lua_pushboolean(lua, false);
		}
		return 1;
	}

	int shouldSendNewAnimation(lua_State* lua)
	{
		if (g_networkController->timeSinceLastAnimationPacket() > 0.033)
		{
			lua_pushboolean(lua, true);
		}
		else
		{
			lua_pushboolean(lua, false);
		}
		return 1;
	}

	int getIP(lua_State * lua)
	{
		std::string line;
		std::ifstream IPFile;
		int offset;
		char* search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern

		system("ipconfig > ip.txt");

		IPFile.open("ip.txt");
		if (IPFile.is_open())
		{
			while (!IPFile.eof())
			{
				getline(IPFile, line);
				if ((offset = (int)line.find(search0, 0)) != std::string::npos)
				{
					//   IPv4 Address. . . . . . . . . . . : 1   
					line.erase(0, 39);
					lua_pushstring(lua, line.c_str());
					IPFile.close();
				}
			}
		}

		remove("ip.txt");

		return 1;
	}

	int shouldSendNewAITransform(lua_State* lua)
	{
		if (g_networkController->timeSinceLastAITransformPacket() > 0.033) // 0.033
		{
			lua_pushboolean(lua, true);
		}
		else
		{
			lua_pushboolean(lua, false);
		}
		return 1;
	}

}