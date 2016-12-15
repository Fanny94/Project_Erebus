#pragma once
#include "HitBox.h"
class OBBColllider : public HitBox
{
public:
	OBBColllider();
	OBBColllider(int transformID);
	//OBBColllider(int transformID);
	~OBBColllider();

	//Overrides
	virtual unsigned int getID() const override; // copy elision makes returning values fast? RVO - NRVO
	virtual int getIDTransform() const override;
	virtual std::vector<unsigned int>* getIDCollisionsRef() override;
	virtual void insertCollisionID(unsigned int collisionID) override;
	virtual void clearCollisionIDs() override;
	virtual void setPos(glm::vec3 pos) override;
	
private:
	glm::vec3 pos;
	glm::vec3 xAxis, yAxis, zAxis;
	float xHalfLength, yHalfLength, zHalfLength;
};

