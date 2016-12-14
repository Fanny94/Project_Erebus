#pragma once
#include "BaseIncludes.h"
#include "Inputs.h"

#define BASE_CAM_SPEED 8.0f
#define TURBO_CAM_SPEED 13.0f

class Camera 
{
public:
	GEAR_API Camera();
	GEAR_API Camera(float FoV, float aspectRatio, float nearPlane, float farPlane, Inputs* in);
	GEAR_API ~Camera();

	GEAR_API void camUpdate(glm::vec3 newPos, glm::vec3 newDir, float dt);
	GEAR_API void follow(glm::vec3 point, glm::vec3 direction, float distance, float angle, float xOffset, float yOffset, float FoV);
	GEAR_API void setCamera(glm::vec3 campos, glm::vec3 lookPos);
	GEAR_API void setPosition(glm::vec3 position);
	GEAR_API void setHeight(float h);
	GEAR_API glm::mat4 getViewPers();
	GEAR_API glm::mat4 getViewMatrix();
	GEAR_API glm::mat4 getProjectionMatrix();
	GEAR_API glm::vec3 getPosition();
	GEAR_API glm::vec3 getDirection();
	
private:
	Inputs *inputs;
	bool freeCam;
	float horizontalAngle;
	float verticalAngle;
	float camSpeed;
	int dir;

	float aspectRatio;
	float nearPlane;
	float farPlane;

	glm::vec3 lookPos;
	glm::vec3 camDirection;
	glm::vec3 camPosition;
	glm::vec3 camUp;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
};