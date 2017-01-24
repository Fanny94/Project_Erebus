#include "LuaCollision.h"

namespace LuaCollision
{
	static CollisionHandler* g_collisionHandler = nullptr;

	void registerFunctions( lua_State* lua, CollisionHandler* handler )
	{
		g_collisionHandler = handler;

		//CollisionHandler
		luaL_newmetatable( lua, "collisionHandlerMeta" );
		luaL_Reg handlerRegs[] =
		{
			{ "AddSphere",			addSphere },
			{ "AddAABB",			addAABB },
			{ "AddRay",				addRay},
			{ "SetLayerCollision",	setLayerCollision },
			{ "PrintCollisions",	printCollisions },
			{ "DrawHitboxes",		drawHitboxes },
			{"DeactivateAllHitboxes", deactivateAllHitboxes},
			{"ActivateAllHitboxes", activateAllHitboxes},
			{"Enable", enableCollisionHandler},
			{"Disable", disableCollisionHandler},
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, handlerRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );

		lua_newtable( lua );
		lua_pushlightuserdata( lua, handler );
		lua_setfield( lua, -2, "__self" );
		luaL_setmetatable( lua, "collisionHandlerMeta" );
		lua_setglobal( lua, "CollisionHandler" );

		//Sphere
		luaL_newmetatable( lua, "sphereColliderMeta" );
		luaL_Reg sphereRegs[] =
		{
			{ "Create",				createSphere },
			{ "GetCollisionIDs",	getCollisionIDs },
			{ "CheckCollision",		checkCollision },
			{ "SetRadius",			setRadius },
			{ "GetID",				getID },
			{ "SetActive", setActive },
			{ "__gc",				destroy },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, sphereRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "SphereCollider" );

		// AABB
		luaL_newmetatable( lua, "aabbColliderMeta" );
		luaL_Reg aabbRegs[] =
		{
			{ "Create",				createAABB },
			{ "GetCollisionIDs",	getCollisionIDs },
			{ "CheckCollision",		checkCollision },
			{ "SetActive", setActive },
			{ "__gc",				destroy },
			{ NULL, NULL }
		};

		luaL_setfuncs( lua, aabbRegs, 0 );
		lua_pushvalue( lua, -1 );
		lua_setfield( lua, -2, "__index" );
		lua_setglobal( lua, "AABBCollider" );

		//Ray
		luaL_newmetatable(lua, "rayColliderMeta");
		luaL_Reg rayRegs[] =
		{
			{ "Create",				createRay },
			{ "GetCollisionIDs",	getRayCollisionIDs },
			{ "CheckCollision",		checkRayCollision },
			{ "SetDirection",			setDirection },
			{ "GetID",				getRayID },
			{ "SetActive", setActive },
			{ "__gc",				destroyRay },
			{ NULL, NULL }
		};

		luaL_setfuncs(lua, rayRegs, 0);
		lua_pushvalue(lua, -1);
		lua_setfield(lua, -2, "__index");
		lua_setglobal(lua, "RayCollider");
		lua_pop(lua, 1);
	}

	int addSphere( lua_State* lua )
	{
		int nargs = lua_gettop( lua );
		if( nargs >= 1 )
		{
			SphereCollider* collider = getSphereCollider( lua, 1 );
			int layer = 0;
			if( nargs >= 2 )
				layer = lua_tointeger( lua, 2 );
			g_collisionHandler->addHitbox( collider, layer );
		}

		return 0;
	}

	int addAABB( lua_State* lua )
	{
		int nargs = lua_gettop( lua );
		if( nargs >= 1 )
		{
			AABBCollider* collider = getAABBCollider( lua, 1 );
			int layer = 0;
			if( nargs >= 2 )
				layer = lua_tointeger( lua, 2 );
			g_collisionHandler->addHitbox( collider );
		}

		return 0;
	}

	int addRay(lua_State * lua)
	{
		int nargs = lua_gettop(lua);
		if (nargs >= 1)
		{
			RayCollider* collider = getRayCollider(lua, 1);
			int layer = 0;
			if (nargs >= 2)
				layer = lua_tointeger(lua, 2);
			g_collisionHandler->addRay(collider);
		}

		return 0;
	}

	int createSphere( lua_State* lua )
	{
		int result = 0;

		/*if( lua_gettop( lua ) >= 6 )
		{
			int colliderID = lua_tointeger( lua, 1 );
			int transformID = lua_tointeger( lua, 2 );
			glm::vec3 position( lua_tonumber( lua, 3 ),
								lua_tonumber( lua, 4 ),
								lua_tonumber( lua, 5 ) );
			float radius = lua_tonumber( lua, 6 );

			SphereCollider* collider = new SphereCollider( colliderID, transformID, position, radius );
			lua_newtable( lua );
			luaL_setmetatable( lua, "sphereColliderMeta" );
			lua_pushlightuserdata( lua, collider );
			lua_setfield( lua, -2, "__self" );

			result = 1;
		}*/

		if( lua_gettop(lua) >= 1 )
		{
			int transformID = lua_tointeger( lua, 1 );

			SphereCollider* collider = new SphereCollider( transformID );
			lua_newtable( lua );
			luaL_setmetatable( lua, "sphereColliderMeta" );
			lua_pushlightuserdata( lua, collider );
			lua_setfield( lua, -2, "__self" );

			result = 1;
		}

		return result;
	}

	int createAABB( lua_State* lua )
	{
		int result = 0;

		/*if( lua_gettop( lua ) >= 8 )
		{
			int colliderID = lua_tointeger( lua, 1 );
			int transformID = lua_tointeger( lua, 2 );
			glm::vec3 minPos( lua_tonumber( lua, 3 ),
								lua_tonumber( lua, 4 ),
								lua_tonumber( lua, 5 ) );
			glm::vec3 maxPos( lua_tonumber( lua, 6 ),
								lua_tonumber( lua, 7 ),
								lua_tonumber( lua, 8 ) );

			AABBCollider* collider = new AABBCollider( colliderID, transformID, minPos, maxPos );
			lua_newtable( lua );
			luaL_setmetatable( lua, "aabbColliderMeta" );
			lua_pushlightuserdata( lua, collider );
			lua_setfield( lua, -2, "__self" );

			result = 1;
		}*/

		if( lua_gettop( lua ) >= 1 )
		{
			int transformID = lua_tointeger( lua, 1 );

			AABBCollider* collider = new AABBCollider( transformID );
			lua_newtable( lua );
			luaL_setmetatable( lua, "aabbColliderMeta" );
			lua_pushlightuserdata( lua, collider );
			lua_setfield( lua, -2, "__self" );

			result = 1;
		}

		return result;
	}

	int createRay(lua_State * lua)
	{
		int result = 0;
		if (lua_gettop(lua) >= 1)
		{
			int transformID = lua_tointeger(lua, 1);

			RayCollider* ray = new RayCollider(transformID,glm::vec3(0,1,0));

			lua_newtable(lua);
			luaL_setmetatable(lua, "rayColliderMeta");
			lua_pushlightuserdata(lua, ray);
			lua_setfield(lua, -2, "__self");

			result = 1;
		}
		return result;
	}

	int destroy( lua_State* lua )
	{
		HitBox* hitbox = getHitBox( lua, 1 );
		delete hitbox;

		return 0;
	}

	int destroyRay(lua_State * lua)
	{
		RayCollider* ray = getRayCollider(lua, 1);
		delete ray;

		return 0;
	}

	int getCollisionIDs( lua_State* lua )
	{
		int result = 0;

		if( lua_gettop( lua ) >= 1 )
		{
			HitBox* hitbox = getHitBox( lua, 1 );

			std::vector<int>* ids = hitbox->getIDCollisionsRef();
			lua_newtable( lua );
			for( int i=0; i<ids->size(); i++ )
			{
				lua_pushnumber( lua, ids->at(i) );
				lua_rawseti( lua, -2, i+1 );
			}

			result = 1;
		}

		return result;
	}

	int getRayCollisionIDs(lua_State * lua)
	{
		int result = 0;

		if (lua_gettop(lua) >= 1)
		{
			RayCollider* ray = getRayCollider(lua, 1);

			std::vector<int>* ids = ray->getIDCollisionsRef();
			lua_newtable(lua);
			for (int i = 0; i<ids->size(); i++)
			{
				lua_pushnumber(lua, ids->at(i));
				lua_rawseti(lua, -2, i + 1);
			}

			result = 1;
		}

		return result;
	}

	int checkCollision( lua_State* lua )
	{
		int result = 0;

		if( lua_gettop( lua ) >= 1 )
		{
			HitBox* hitbox = getHitBox( lua, 1 );

			lua_pushboolean( lua, hitbox->checkCollision() );
			result = 1;
		}

		return result;
	}

	int checkRayCollision(lua_State * lua)
	{
		int result = 0;

		if (lua_gettop(lua) >= 1)
		{
			RayCollider* ray = getRayCollider(lua, 1);

			lua_pushboolean(lua, ray->checkCollision());
			result = 1;
		}

		return result;
	}

	int setRadius( lua_State* lua )
	{
		if( lua_gettop( lua ) >= 2 )
		{
			SphereCollider* collider = (SphereCollider*)getHitBox( lua, 1 );
			float radius = lua_tonumber( lua, 2 );

			collider->setRadius( radius );
		}

		return 0;
	}

	int setActive(lua_State * lua)
	{
		if (lua_gettop(lua) >= 2)
		{
			HitBox* hitbox = getHitBox(lua, 1);
			bool active = lua_toboolean(lua, 2);

			hitbox->setActive(active);
		}
		return 0;
	}

	int setRayActive(lua_State * lua)
	{
		if (lua_gettop(lua) >= 2)
		{
			RayCollider* ray = getRayCollider(lua, 1);
			bool active = lua_toboolean(lua, 2);

			ray->setActive(active);
		}
		return 0;
	}

	int setDirection(lua_State * lua)
	{
		if (lua_gettop(lua) >= 4)
		{
			RayCollider* ray = (RayCollider*)getRayCollider(lua, 1);
			float x = lua_tonumber(lua, 2);
			float y = lua_tonumber(lua, 3);
			float z = lua_tonumber(lua, 4);

			ray->setDirection(glm::vec3(x,y,z));
		}

		return 0;
	}

	int setLayerCollision( lua_State* lua )
	{
		if( lua_gettop( lua ) >= 3 )
		{
			int layer1 = lua_tointeger( lua, 1 );
			int layer2 = lua_tointeger( lua, 2 );
			bool canCollide = lua_toboolean( lua, 3 );

			g_collisionHandler->setLayerCollisionMatrix( layer1, layer2, canCollide );
		}

		return 0;
	}

	int printCollisions( lua_State* lua )
	{
		g_collisionHandler->printCollisions();
		return 0;
	}

	int drawHitboxes( lua_State* lua )
	{
		g_collisionHandler->drawHitboxes();
		return 0;
	}

	int deactivateAllHitboxes(lua_State * lua)
	{
		g_collisionHandler->deactiveteAllHitboxes();
		return 0;
	}

	int activateAllHitboxes(lua_State * lua)
	{
		g_collisionHandler->activeteAllHitboxes();
		return 0;
	}

	int enableCollisionHandler(lua_State * lua)
	{
		g_collisionHandler->setEnabled(true);
		return 0;
	}

	int disableCollisionHandler(lua_State * lua)
	{
		g_collisionHandler->setEnabled(false);
		return 0;
	}

	int getID( lua_State* lua )
	{
		int result = 0;

		if( lua_gettop( lua ) >= 1 )
		{
			HitBox* hitbox = getHitBox( lua, 1 );

			lua_pushnumber( lua, hitbox->getID() );
			result = 1;
		}

		return result;
	}

	int getRayID(lua_State * lua)
	{
		int result = 0;

		if (lua_gettop(lua) >= 1)
		{
			RayCollider* ray = getRayCollider(lua, 1);

			lua_pushnumber(lua, ray->getID());
			result = 1;
		}

		return result;
	}

	HitBox* getHitBox( lua_State* lua, int index )
	{
		lua_getfield( lua, index, "__self" );
		return (HitBox*)lua_touserdata( lua, -1 );
	}

	SphereCollider* getSphereCollider( lua_State* lua, int index )
	{
		lua_getfield( lua, index, "__self" );
		return (SphereCollider*)lua_touserdata( lua, -1 );
	}

	AABBCollider* getAABBCollider( lua_State* lua, int index )
	{
		lua_getfield( lua, index, "__self" );
		return (AABBCollider*)lua_touserdata( lua, -1 );
	}
	RayCollider * getRayCollider(lua_State * lua, int index)
	{
		lua_getfield(lua, index, "__self");
		return (RayCollider*)lua_touserdata(lua, -1);
	}
}