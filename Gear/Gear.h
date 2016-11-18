#pragma once
#include "BaseIncludes.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "RenderQueue.h"
#include "Particles.h"

namespace Gear
{
	class GearEngine {
	public:
		GEAR_API GearEngine();
		GEAR_API ~GearEngine();
		GEAR_API void draw(Camera* camera);
		GEAR_API void drawParticles(Particle* particles);

		GEAR_API bool isRunning();

		// TEMPORARY: Vi ska inte ha det s�h�r
		// F�r att rita ut modellen fr�n importern
		RenderQueue renderQueue;
		std::vector<RenderQueueElement*> renderElements;

	private:
		//Window* window;
		std::vector<ShaderProgram*> allShaders;
	};
	GEAR_API double Add(double a, double b);
}