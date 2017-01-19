#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "glm\gtx\rotate_vector.hpp"
class HitBox
{
	
public:
	HitBox();
	HitBox(unsigned int ID, int IDTransform);
	HitBox(int IDTransform);
	virtual ~HitBox();
	virtual unsigned int getID() const = 0; // copy elision makes returning values fast? RVO - NRVO
	virtual int getIDTransform() const = 0;
	virtual std::vector<unsigned int>* getIDCollisionsRef() = 0;
	virtual void insertCollisionID(unsigned int collisionID) = 0;
	virtual void clearCollisionIDs() = 0;
	virtual void setPos(glm::vec3 pos) = 0;
	virtual void setIDTransform(unsigned int ID);
	virtual void setID(unsigned int ID);
	virtual bool checkCollision();
	virtual bool checkSpecificCollision(unsigned int target);
	virtual void setActive(bool active);
	virtual bool isActive();

protected:
	int ID, IDTransform;
	std::vector<unsigned int> IDCollisions;
	const unsigned short RESERVE_COLLISIONS = 10; // hur m�nga collisions tror vi en enda hitbox max har under en frame
	bool active;

};

