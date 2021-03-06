#include "ParticleSystem.h"
#include <math.h>

namespace Gear
{
	ParticleSystem::ParticleSystem()
		: removed( false )
	{
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] newEmitterPos;	
		delete[] particleEmitters;		
	}

	ParticleSystem::ParticleSystem(std::string path, Importer::Assets * assets)
		: removed( false )
	{
		isActive = false;

		this->systemPos = { 0, 0, 0 };
		this->newEmitterPos = new glm::vec3[3];
		this->emitterPos = glm::vec3(0, 0, 0);
		this->dir = glm::vec3(0, 0, 0);
		this->nrActive = 0;

		this->v1 = { 1, 0, 0 };
		this->v2 = { 0, 1, 0 };
		this->v3 = { 0, 0, 1 };

		FILE* file;
		file = fopen(path.c_str(), "rb");
		Emitter p;
		int n;

		if (file)
		{
			fread(&n, sizeof(int), 1, file);
			nrOfEmitters = n;
			particleEmitters = new ParticleEmitter[nrOfEmitters];
			for (int i = 0; i < nrOfEmitters; i++)
			{
				fread(&p, sizeof(Emitter), 1, file);
				particleEmitters[i].emitterInit(p, assets);
			}
			fclose(file);
		}
	}

	GEAR_API void ParticleSystem::update(const float &dt)
	{
		if (isActive)
		{
			for (int i = 0; i < nrOfEmitters; i++)
			{
				emitterPos = particleEmitters[i].localPos;

				newEmitterPos[0] = v1 * emitterPos.x;
				newEmitterPos[1] = emitterPos.y * v2;
				newEmitterPos[2] = emitterPos.z * v3;
				emitterPos = systemPos + newEmitterPos[0] + newEmitterPos[1] + newEmitterPos[2];

				particleEmitters[i].setEmitterPos(emitterPos);

				nrActive -= particleEmitters[i].update(dt);
				
			}	
			if (nrActive == 0)
			{
				isActive = false;
			}
		}
	}

	GEAR_API int ParticleSystem::getNrOfEmitters()
	{
		return this->nrOfEmitters;
	}

	GEAR_API void ParticleSystem::setSystemPos(glm::vec3 pos)
	{
		this->systemPos = pos;
	}

	GEAR_API void ParticleSystem::activate()
	{
		isActive = true;
		alive = true;
		nrActive = nrOfEmitters;
		for (size_t i = 0; i < nrOfEmitters; i++)
		{
			particleEmitters[i].isActive = true;
			particleEmitters[i].alive = true;
		}
	}

	GEAR_API void ParticleSystem::deActivate()
	{
		alive = false;

		for (size_t i = 0; i < nrOfEmitters; i++)
		{
			particleEmitters[i].alive = false;
		}
	}
	
	GEAR_API void ParticleSystem::setDirection(glm::vec3 direction)
	{
		dir = direction;

		for (int i = 0; i < nrOfEmitters; i++)
		{
			particleEmitters[i].setDirection(dir);
		}
		
	}

	GEAR_API int ParticleSystem::getNrOfActive()
	{
		return nrActive;
	}

	GEAR_API void ParticleSystem::explode(glm::vec3 position)
	{
		for (size_t i = 0; i < nrOfEmitters; i++)
		{
			particleEmitters[i].explode2(position);
		}
		nrActive = nrOfEmitters;
		isActive = true;
		alive = false;
	}
	GEAR_API void ParticleSystem::setExtroversion(bool yesNo)
	{
		for (size_t i = 0; i < nrOfEmitters; i++)
		{
			particleEmitters[i].setExtrovert(yesNo);
		}
	}
	GEAR_API void ParticleSystem::setFocus(float focus)
	{
		for (size_t i = 0; i < nrOfEmitters; i++)
		{
			particleEmitters[i].setFocus(focus);
		}
	}

	GEAR_API void ParticleSystem::setRemoved(bool r)
	{
		removed = r;
	}

	GEAR_API bool ParticleSystem::getRemoved() const
	{
		return removed;
	}
}