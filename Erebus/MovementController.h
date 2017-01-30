#pragma once

#include "CollisionHandler.h"
#include <iostream>

class MovementController
{
public:
	MovementController();
	~MovementController();

	void move( glm::vec3 distance );
	void update();

	//Setters
	void setHitbox( HitBox* hitbox );
	void setTransform( Transform* transform );
	void setCollisionLayer( CollisionLayers* layer );
	void setHeightmap(HeightMap* heightmap);

private:
	HitBox* hitbox;
	Transform* transform;
	glm::vec3 movement;
	CollisionLayers* collisionLayer;
	HeightMap* heightmap;
};