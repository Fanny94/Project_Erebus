#pragma once

#include "RenderQueueElement.h"
#include "ShaderProgram.h"
#include "Camera.h"
//#include "Model.h"
#include "ParticleSystem.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Material.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include "Animation.h"
#include "Light.h"
#include "WorkQueue.h"

using namespace Importer;
struct ModelInstance
{
	ModelAsset* asset;
	//Material material;
	std::vector<int> worldIndices;
};

struct AnimatedInstance
{
	ModelAsset* asset;
	//Material material;
	std::vector<int> worldIndices;
	std::vector<Animation*> animations;
};

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	void init();
	void updateUniforms(Camera* camera);
	void updateUniforms(Camera* camera, ShaderType shader);
	void process(std::vector<RenderQueueElement*> &elements);
	void allocateWorlds(int n);
	void update(int ntransforms, TransformStruct* theTrans, int nanimations, Animation* animations);
	//GEAR_API int modelAdded(Model* model);
	int addModelInstance(ModelAsset* asset);
	int generateWorldMatrix();
	ShaderProgram* getShaderProgram(ShaderType type);
	// TEMP:
	std::vector<Gear::ParticleSystem*> particleSystem;

	void particleSort(std::vector<Gear::ParticleSystem*>* particleSystems, glm::vec3 cameraPos);

	void forwardPass(std::vector<ModelInstance>* staticModels, std::vector<ModelInstance>* dynamicModels);
	void particlePass(std::vector<Gear::ParticleSystem*>* particleSystems);
	//void geometryPass(std::vector<ModelInstance>* dynamicModels);
	void geometryPass( std::vector<ModelInstance>* dynamicModels, std::vector<AnimatedInstance>* animatedModels );
	void geometryPass(std::vector<ModelInstance>* dynamicModels, std::vector<AnimatedInstance>* animatedModels, Lights::DirLight light);
	void pickingPass(std::vector<ModelInstance>* dynamicModels);

	void setWorkQueue( WorkQueue* workQueue );

private:
	int currentShader = 0;
	int currentTexture = 0;
	int currentCallType = 0;
	ShaderProgram* allShaders[ShaderType::NUM_SHADER_TYPES];
	//std::vector<Model*> allModels;
	std::vector<ModelInstance> instances;
	std::vector<ModelInstance> staticInstances;
	glm::mat4* worldMatrices;
	glm::mat4* tempMatrices;
	glm::mat4* jointMatrices;
	int nrOfWorlds;
	int totalWorlds;
	WorkQueue* work;

	double freq;

	struct AsyncTransformData
	{
		TransformStruct* transforms;
		glm::mat4* worldMatrices;
		int first, last;
	};
	AsyncTransformData asyncTransformData[MAX_THREADS];
	static void asyncTransformUpdate( void* args );

private:
	void configure(RenderQueueId &id, GLuint &shaderProgramId);

};