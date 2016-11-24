#pragma once
#include "BaseIncludes.h"

class Particle
{
public:

	GEAR_API Particle();
	GEAR_API Particle(glm::vec3 pos, GLfloat duration, GLfloat speed, GLfloat angle, glm::vec3 color);
	GEAR_API ~Particle();

private:

	glm::vec3 pos;
	GLfloat duration;
	GLfloat speed;
	GLfloat angle;
	glm::vec3 color;
};