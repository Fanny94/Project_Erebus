#pragma once
#include "BaseIncludes.h"
#include "Model.h"


class Magic {
private:
	glm::vec3 position;
	glm::vec3 direction;
	float scale;
	float speed;
	Model* model;
public:
	Magic();
	~Magic();
	void spawn(glm::vec3 pos, glm::vec3 dir, float magicScale, float magicSpeed);
};