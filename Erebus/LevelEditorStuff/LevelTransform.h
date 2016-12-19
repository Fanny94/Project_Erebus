#pragma once
#include "LevelActorComponent.h"
#include "BaseIncludes.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include "../Transform.h"
class LevelTransform : public LevelActorComponent
{
public:
	static const char* name;
	LevelTransform();
	~LevelTransform();
	void initialize(tinyxml2::XMLElement* element) override;
	void update(float deltaTime) override;
	std::string getName() override;
	tinyxml2::XMLElement* toXml(tinyxml2::XMLDocument* doc) override;
	void postInitialize() override;
	void setParent(LevelActor* parent);


private:
	glm::vec3 position;
	glm::vec3 rotation;
	Transform* transformRef;
	
};

