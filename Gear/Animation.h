#pragma once
#include "ModelAsset.h"
#include <glm\common.hpp>
#include "BaseIncludes.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
/*
Hold the animation stack
Update animations
Included in every model

Have the ability to interpolate between two animation states
Which means that interpolation needs to be implemented individually for both animation states,
and then interpolation needs to be done between those two animation states.
*/
#define MAXJOINTCOUNT 64
using namespace Importer;
class Animation
{
public:
	GEAR_API Animation();
	GEAR_API virtual ~Animation();
	GEAR_API void setAsset(Importer::ModelAsset* asset);
	GEAR_API virtual void updateAnimation(float dt, int layer);

	GEAR_API virtual std::vector<sKeyFrame> updateAnimationForBlending(float dt, int layer, float& animTimer);
	//The state is an enum defined for each subclass of Animation
	GEAR_API virtual void updateState(float dt, int state, int animationSegment);

	/*
	The number of parts is equal to 
	*/
	GEAR_API virtual void setAnimationSegments(int numberOfSegments);
	/*
	Set transition times for all possible To and From state combinations by making a
	float matrix/table constructed in this manner:
	
	  FROM >>>>>>>>>>>
	TO	  0  1  2   3 ... <-- these numbers are animation states, defined for each model
	v   0 t1 t5 t9  t13     <-- t for transition time defined by user
	v   1 t2 t6 t10 t14
	v   2 t3 t7 t11 t15 
	v   3 t4 t8 t12 t16
	   ...
	FROM means "the animation state you are transitioning from"
	TO means "the animation state you are transitioning to"

	The length of the row and column is the same as the number of animationStates that you 
	send in to this "setTransitionTimes" function.
	*/
	GEAR_API void setTransitionTimes(float* transitionTimeArray, int numStates);

	GEAR_API virtual void setStates(int numStates);

	GEAR_API virtual void assembleAnimations();

	GEAR_API virtual glm::mat4x4* getShaderMatrices();

protected:
	void blendAnimations(int blendTo, int blendFrom, float& transitionTimer, float dt);
	Importer::sKeyFrame interpolateKeys(Importer::sKeyFrame overKey, Importer::sKeyFrame underKey, float& animTimer);
	Importer::sKeyFrame interpolateKeysForBlending(Importer::sKeyFrame to, Importer::sKeyFrame from);
	
	void updateJointMatrices(std::vector<sKeyFrame>& keyList);
	void myLerp(float arr1[3], float arr2[3], float fillArr[3], float iVal);
	void convertToRotMat(float in[3], glm::mat4* result);
	void convertToTransMat(float inputArr[3], glm::mat4* result);
	void convertToScaleMat(float inputArr[3], glm::mat4* result);
	
	float animationTimer;

	float* transitionTimeArray;
	int transitionTimeArraySize;

	int numStates;
	int oldTo = -1337;
	int oldFrom = -1337;
	int animationSegments;
	std::vector<sKeyFrame> blendFromKeys;
	std::vector<sKeyFrame> blendToKeys;

	//Animation blending variables, one per animationPart;
	std::vector<float> fromAnimationTimers;
	std::vector<float> toAnimationTimers;
	std::vector<float> transitionMaxTimes;
	std::vector<float> transitionTimers;
	//Animationtimer 
	std::vector<float> animationTimers;

	//List holding "final" jointmatrices (one per animationSegment) before they're added together
	//used like: animationMatrixLists[animationSegment][jointIdx]
	std::vector<glm::mat4x4*> animationMatrixLists;

	float fromAnimationTimer;
	float toAnimationTimer;
	float transitionMaxTime = 0;

	//When this timer is set, make sure it doesn't get reset when doing the same blend again
	float transitionTimer = 0;
	
	std::vector<int> animationStack;
	std::vector<std::vector<int>> animationStacks;
	bool isTransitionComplete = true;

	glm::mat4x4 shaderMatrices[MAXJOINTCOUNT];
	Importer::ModelAsset* asset;
	std::vector<glm::mat4> animMatrix;
	std::vector<sKeyFrame> finalList;
};