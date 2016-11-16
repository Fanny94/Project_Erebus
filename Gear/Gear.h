#pragma once
#include "Window.h"
#include "BaseIncludes.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

namespace Gear
{
	class GearEngine {
	public:
		GEAR_API GearEngine();
		GEAR_API ~GearEngine();
		GEAR_API void draw();
		GEAR_API bool isRunning();

		// TEMPORARY: Vi ska inte ha det s�h�r
		// F�r att rita ut modellen fr�n importern
		RenderQueueElement* model;

	private:
		Window* window;
		std::vector<ShaderProgram*> allShaders;
	};
	GEAR_API double Add(double a, double b);
}