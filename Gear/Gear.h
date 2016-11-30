#pragma once
#include "BaseIncludes.h"
#include "Camera.h"
//#include "Model.h"
#include "Particles.h"
#include "RenderQueue.h"
#include "Particles.h"
#include "Inputs.h"

namespace Gear
{
	class GearEngine {
	public:
		GEAR_API GearEngine();
		GEAR_API ~GearEngine();
		GEAR_API void draw(Camera* camera);
		

		GEAR_API bool isRunning();

		// TEMPORARY: Vi ska inte ha det s�h�r
		// F�r att rita ut modellen fr�n importern
		RenderQueue renderQueue;
		std::vector<RenderQueueElement*> renderElements;
	/*	std::vector<RenderQueueElement*> renderParticles;*/
	/*	std::vector<ShaderProgram*> allShaders;*/

	private:
		//Window* window;
	};
}