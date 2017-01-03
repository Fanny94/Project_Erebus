#pragma once
#include "AABBCollider.h"
#include "OBBColllider.h"
#include "SphereCollider.h"
#include "glm\glm.hpp"
class CollisionChecker
{
private:

	//Counters
	int sphereCollisionCounter = 0;
	int aabbCollisionCounter = 0;
	int sphereToAabbCollisionCounter = 0;
	int obbCollisionCounter = 0;
	int obbToAabbCollisionCounter = 0;
	int obbToSphereCollisionCounter = 0;

public:
	CollisionChecker();
	~CollisionChecker();

	bool collisionCheck(SphereCollider* sphere1, SphereCollider* sphere2); // http://www.miguelcasillas.com/?p=9
	bool collisionCheck(AABBCollider* aabb1, AABBCollider* aabb2); // https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
	bool collisionCheck(SphereCollider* sphere, AABBCollider* aabb); // https://studiofreya.com/3d-math-and-physics/sphere-vs-aabb-collision-detection-test/
	bool collisionCheck(OBBColllider* collider, OBBColllider* collider2); //http://www.jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf
	bool collisionCheck(OBBColllider* collider, AABBCollider* collider2);

	//helper functions
	float closestDistanceAabbToPoint(const float& point, const float aabbMin, const float aabbMax);
	float SquaredDistancePointToAabb(AABBCollider* aabb, SphereCollider* sphere);

	void resetCounters();

	//getters
	int getSphereCollisionCounter();
	int getAabbCollisionCounter();
	int getSphereToAabbCollisionCounter();
	int getObbCollisionCounter();
	int getObbToAabbCollisionCounter();
	int getObbToSphereCollisionCounter();
	int getTotalCollisionCounter();
};

