#pragma once
#include"BaseIncludes.h"
#include"AGI.h"
class InfluenceNode
{
public:
	AGI_API InfluenceNode()
	{
		pos = glm::vec2(0, 0);
		strength = 1;
	};
	AGI_API InfluenceNode(glm::vec2 inPos, float inStrength)
	{
		pos = inPos;
		strength = inStrength;
	};
	AGI_API ~InfluenceNode()
	{

	};

	AGI_API glm::vec2 getPos()
	{
		return pos;
	}

	AGI_API float getStrength()
	{
		return strength;
	}

	AGI_API void setPos(glm::vec2 inPos)
	{
		pos = inPos;
	}

	AGI_API void setStrength(float inStrength)
	{
		strength = inStrength;
	}
private:
	glm::vec2 pos;
	float strength;
};