#include "particleEmitter.h"

#include <math.h>

namespace Gear
{
	ParticleEmitter::ParticleEmitter() : particleSize(1.0)
	{

	}

	void ParticleEmitter::emitterInit(Emitter emitter, Importer::Assets* assets)
	{		
		timer = 0;
		isActive = false;
		nrOfActiveParticles = 0;
		this->maxParticles = emitter.numOfParticles;
		this->allParticles = new Partikel[maxParticles];
		this->particlePos = new SendStruct[maxParticles];
		this->lifeTime = emitter.lifeTime;
		this->partSpeed = emitter.speed;
		this->particleRate = emitter.particleRate;
		this->partPerRate = emitter.partPerRate;
		this->gravityFactor = emitter.gravity;
		this->focus = emitter.focusSpread;
		this->particleSize = emitter.particleSize;
		this->direction = { emitter.dirX, emitter.dirY, emitter.dirZ };
		this->textureAssetParticles = assets->load<Importer::TextureAsset>("Textures/" + std::string(emitter.textureName));
		for (size_t i = 0; i <maxParticles; i++)
		{
			particlePos[i].size = 1.0;
		}
		shrinkage = -1;
	}

	ParticleEmitter::~ParticleEmitter()
	{	
		delete[] this->allParticles;
		delete[] this->particlePos;
	}

	void ParticleEmitter::spawn(float dt)
	{
		if (alive)
		{
			timer += dt;
			if (timer > particleRate)
			{
				glm::vec3 tempVec = this->position + direction * focus; //emit direction
				glm::vec3 temp2;
				int i = 0;
				while (nrOfActiveParticles < maxParticles && partPerRate > i++)
				{
					particlePos[nrOfActiveParticles].pos = this->position;
					allParticles[nrOfActiveParticles].lifeSpan = this->lifeTime;
					temp2 = glm::normalize(glm::vec3((rand() % 20 - 10), (rand() % 20 - 10), (rand() % 20 - 10))) + tempVec;
					allParticles[nrOfActiveParticles++].direction = glm::normalize(temp2 - this->position);
				}
				timer = 0;
			}
		}
	}

	bool ParticleEmitter::update(const float &dt)
	{
		if (isActive)
		{
			spawn(dt);

			float randomSpeed;
			for (int i = 0; i < nrOfActiveParticles; i++)
			{
				allParticles[i].lifeSpan -= dt;
				if (allParticles[i].lifeSpan > 0.0)
				{
					allParticles[i].direction.y += gravityFactor * dt;
					randomSpeed = rand() % (int)partSpeed;
					particlePos[i].pos += allParticles[i].direction * randomSpeed * dt;
					particlePos[i].size += shrinkage * dt;
				}
				else
				{
					particlePos[i] = particlePos[nrOfActiveParticles - 1];
					allParticles[i] = allParticles[--nrOfActiveParticles];
					particlePos[i].size = this->particleSize;
					if (nrOfActiveParticles <= 0)
						isActive = false;
				}
			}
		}
		return !isActive;
	}

	void ParticleEmitter::explode()
	{
		nrOfActiveParticles = 0;
		for (int i = 0; i < maxParticles; i++)
		{
			particlePos[i].pos = this->position;
			particlePos[i].size = this->particleSize;
			allParticles[i].lifeSpan = this->lifeTime;
			allParticles[i].direction = glm::normalize(glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5));
			allParticles[i].direction *= rand() % (int)partSpeed;
			nrOfActiveParticles = i;
		}
		isActive = true;
		alive = false;
	}

	void ParticleEmitter::resetEmitter()
	{
		for (int i = 0; i < this->maxParticles; i++)
		{
			particlePos[i].pos = this->position;
			allParticles[i].lifeSpan = 0;
			allParticles[i].direction = { 0, 0, 0 };
		}
		nrOfActiveParticles = 0;
	}

	void ParticleEmitter::setEmitterPos(glm::vec3 pos)
	{
		this->position = pos;
	}

	void ParticleEmitter::setDirection(glm::vec3 dir)
	{
		this->direction = dir;
	}

	GEAR_API int ParticleEmitter::getNrOfActiveParticles()
	{
		return this->nrOfActiveParticles;
	}

	GEAR_API SendStruct* ParticleEmitter::getPositions()
	{
		return this->particlePos;
	}

	GEAR_API Importer::TextureAsset * ParticleEmitter::getTexture()
	{
		return this->textureAssetParticles;
	}
}