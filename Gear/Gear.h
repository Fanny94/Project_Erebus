#pragma once
#include "BaseIncludes.h"
#include "Camera.h"
//#include "Model.h"
#include "RenderQueue.h"
#include "Light.h"
#include "Inputs.h"
#include "staticNonModels.h"
#include "Debug.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "Material.h"
#include "DebugHandler.h"
#include "Skybox.h"

namespace Gear
{
	class Dummy {};

	class Light {};

	class GearEngine {
	public:
		GEAR_API GearEngine();
		GEAR_API ~GearEngine();
		GEAR_API void draw(Camera* camera, std::vector<ModelInstance>* instances);

		GEAR_API bool isRunning();
		//Temporay debug function
		GEAR_API void setDrawMode(int drawMode);
		
		GEAR_API void addStaticNonModel(staticNonModels* model);

		GEAR_API void addDebugger(Debug* debugger);

		//--TODO: Implement API--
		GEAR_API void bindTransforms(TransformStruct** theTrans, int* n);

		GEAR_API void addModelInstance(ModelAsset* asset);

		GEAR_API glm::vec2 getTextDimensions( const char* text );
		GEAR_API void print(const std::string	&s, 
							const float			&baseX, 
							const float			&baseY,
							const float			&scale = 1.0f, 
							const glm::vec4		&color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		GEAR_API void showImage(const sScreenImage & quad, Importer::TextureAsset* texture);
		GEAR_API void showImage(const glm::vec2 &pos, 
								const float &width, 
								const float &height, 
								Importer::TextureAsset* texture);

		GEAR_API void queueModels(std::vector<ModelInstance>* models);
		GEAR_API void queueDynamicModels(std::vector<ModelInstance>* models);
		GEAR_API void queueAnimModels(std::vector<AnimatedInstance>* models);
		GEAR_API void queueParticles(std::vector<Gear::ParticleSystem*> &ps);
		GEAR_API void queueLights(std::vector<Lights::PointLight>* lights);
		GEAR_API void queueLights(Lights::DirLight* lights);
		GEAR_API void draw(Camera* camera);

		GEAR_API void allocateWorlds(int n);
		GEAR_API int generateWorldMatrix();

		GEAR_API void setFont(FontAsset* font);
		GEAR_API void effectPreProcess();
		//----------------------

	private:
		const int NUM_LIGHTS = 50; //number of lights should be the same in lightPass.frag
		const glm::vec3 LIGHT_MIN_BOUNDS = glm::vec3(-0.0f, 10.0f, -0.0f); //the bounds that the lights can get randomly positioned at
		const glm::vec3 LIGHT_MAX_BOUNDS = glm::vec3(255.0f, 25.0f, 255.0f);

		GLuint lightBuffer = 0; //StorageBuffer for point lights
		int drawMode = 1; //Texture draw mode

		//Screen quad data
		GLuint quadVAO = 0;
		GLuint quadVBO;

		//Renderer
		RenderQueue queue;

		std::vector<Lights::DirLight> dirLights;

		//Framebuffers
		ShaderProgram shadowMap;
		ShaderProgram shadowMapTemp;
		ShaderProgram gBuffer;
		ShaderProgram particleFBO;

		//Shaders
		ShaderProgram *quadShader;
		ShaderProgram *lightPassShader;
		ShaderProgram *blurShader;
		ShaderProgram *effectShader;
		ShaderProgram *gloomCompute;
		GLuint gloomTexture;

		//Models
		std::vector<staticNonModels*> statModels;
		std::vector<ModelInstance>* staticModels;
		std::vector<ModelInstance>* dynamicModels;
		std::vector<AnimatedInstance>* animatedModels;
		std::vector<Gear::ParticleSystem*>* particleSystem;

		//Transform data
		TransformStruct** allTrans;
		float** transformArray;		//Sekvens: {pos0x, pos0y, pos0z, rot0x, rot0y, rot0z, pos1x...}
		int** transformIndexArray;
		bool** transformActiveArray;
		int* transformCount;
		glm::vec3* transformLookAts;

		//Skybox object
		Skybox skybox;

		//Default values, to avoid nullptrs
		std::vector<ModelInstance> defaultModelList = std::vector<ModelInstance>(0);

		//Debug Draw handler
		DebugHandler* debugHandler;

		TextRenderer text;
		ImageRenderer image;

		void lightPass(Camera* camera, Camera* tempCam); //Final lighting pass
		void pickingPass();
		void drawQuad(); //Draw Screen quad
		void updateDebug(Camera* camera);
		void BlurFilter(ShaderProgram * dest, ShaderProgram * source, glm::vec3 blurScale); //Blur texture post processing
		void shadowMapBlur(ShaderProgram * dest, ShaderProgram * source, float blurAmount); //ShadowMap bluring
		void frameBufferInit(); //Init all framebuffers
		void shaderInit();
		void lightInit();
		void skyboxInit();
	};
}