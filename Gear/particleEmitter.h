#pragma once
#include "BaseIncludes.h"
#include "TextureAsset.h"

struct Partikel
{
	glm::vec3 direction;
	float lifeSpan;
};

struct SendStruct
{
	glm::vec3 pos;
	float size;
};

struct Emitter
{
	int numOfParticles;
	float posX, posY, posZ;
	float lifeTime;
	float speed;
	float particleRate;
	int partPerRate;
	float gravity;
	float focusSpread;
	float particleSize;
	float dirX, dirY, dirZ;
	char textureName[32];
};

namespace Gear
{
	class ParticleEmitter
	{

	public:
		GEAR_API ParticleEmitter();
		GEAR_API ~ParticleEmitter();
		GEAR_API void emitterInit(Emitter emitter, Importer::Assets* assets);

		GEAR_API bool update(const float & dt);
		GEAR_API void explode();
		GEAR_API void setEmitterPos(glm::vec3 pos);
		GEAR_API void setDirection(glm::vec3 dir);
		GEAR_API int getNrOfActiveParticles();
		GEAR_API SendStruct* getPositions();
		GEAR_API Importer::TextureAsset* getTexture();
		GEAR_API void resetEmitter();
		
		bool isActive;
		glm::vec3 localPos;
		bool alive;
	private:
		float timer;
		Partikel* allParticles;
		float shrinkage;
		SendStruct* particlePos;
		int nrOfActiveParticles;
		float particleSize;

		Importer::TextureAsset* textureAssetParticles;	
		glm::vec3 position;	
		int maxParticles;
		float lifeTime;
		float partSpeed;
		float particleRate;
		int partPerRate;
		float focus;
		glm::vec3 direction;
		float gravityFactor;

		void spawn(float dt);
	};
}

