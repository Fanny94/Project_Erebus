#include "Gear.h"

struct ScreenVertex
{
	float x, y, s, t;
};
GLuint testScreen;
namespace Gear
{
	GearEngine::GearEngine()
	{
		glewInit();
		renderQueue.init();
	}

	GearEngine::~GearEngine()
	{

		glfwTerminate();
		for (size_t i = 0; i < statModels.size(); i++) {
			delete statModels.at(i);
		}

	}

	void GearEngine::draw(Camera* camera, std::vector<ModelInstance>* instances)
	{
		/* Render here */

		//TEMP--------
		//renderElements[0]->id = RenderQueueId(FORWARD, 0);
		//renderElements[1]->id = RenderQueueId(FORWARD, 0);
		//renderElements[3]->id = RenderQueueId(FORWARD, 0);
		//------------

		renderQueue.updateUniforms(camera);
		renderQueue.update(transformArray, transformIndexArray, *transformCount, transformLookAts);
		renderQueue.draw(instances);
		GLfloat positions[] = { 0.5, 0.5, 0.0 };



		//renderQueue.process( renderElements );
		for (size_t i = 0; i < statModels.size(); i++)
		{
			ShaderProgram* tempProgram = statModels.at(i)->getShaderProgram();
			tempProgram->use();
			tempProgram->addUniform(camera->getProjectionMatrix(), "projectionMatrix");
			tempProgram->addUniform(camera->getViewMatrix(), "viewMatrix");
			tempProgram->addUniform(camera->getPosition(), "viewPos");
			tempProgram->addUniform(statModels.at(i)->getWorldMat(), "worldMatrix");
			statModels.at(i)->draw();
			tempProgram->unUse();
		}

	}

	bool GearEngine::isRunning(){
		return true;//window->isWindowOpen();
	}
	
	void GearEngine::addStaticNonModel(staticNonModels* model) {
		model->addShaderProgramRef(this->renderQueue.getShaderProgram(model->getShaderType()));
		this->statModels.push_back(model);
	}

	void GearEngine::bindTransforms(float* pos, int* indices, int* n, glm::vec3* lookAts)
	{
		transformArray = pos;
		transformIndexArray = indices;
		transformCount = n;
		transformLookAts = lookAts;
	}

	void GearEngine::addModelInstance(ModelAsset* asset)
	{

	}

	void GearEngine::queueStaticModels(std::vector<ModelInstance*> &models)
	{

	}

	void GearEngine::queueDynamicModels(std::vector<ModelInstance*> &models)
	{

	}

	void GearEngine::queueAnimModels(std::vector<Dummy*> &models)
	{

	}

	void GearEngine::queueParticles(std::vector<Particle*> &particles)
	{

	}

	void GearEngine::queueLights(std::vector<Light*> &lights)
	{

	}

	void GearEngine::draw()
	{

	}

	void allocateWorlds(int n)
	{

	}
}