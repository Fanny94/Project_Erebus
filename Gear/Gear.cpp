#include "Gear.h"
//Temp
#include <random>
#include <math.h>

struct ScreenVertex
{
	float x, y, s, t;
};

namespace Gear
{
	GearEngine::GearEngine()
	{
		glewInit();
		queue.init();
		text.init(WINDOW_WIDTH, WINDOW_HEIGHT);
		image.init(WINDOW_WIDTH, WINDOW_HEIGHT);
		worldImage.init(WINDOW_WIDTH, WINDOW_HEIGHT);
		skybox = new Skybox();

		staticModels = &defaultModelList;
		dynamicModels = &defaultModelList;

		frameBufferInit();
		shaderInit();
		lightInit();
		skyboxInit();

		debugHandler = new DebugHandler();
		debugHandler->addDebuger(Debugger::getInstance());
		shadow.Init(WINDOW_HEIGHT, WINDOW_HEIGHT, dirLights[0]);

	}

	GearEngine::~GearEngine()
	{
		glfwTerminate();
		delete debugHandler;
		delete skybox;
	}
#pragma region init functions
	void GearEngine::lightInit()
	{
		
		this->updateLightQueue.push_back(l);

		//Generate buffers
		glGenBuffers(1, &lightBuffer); //Generate buffer to light data

									   //bind light buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind buffer
		glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_LIGHTS * sizeof(Lights::PointLight), 0, GL_DYNAMIC_DRAW); //allocate size of buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); //unbind buffer

		resetLightbuffer();

		Lights::DirLight dirLight; //add one dir light
		dirLight.direction = glm::normalize(glm::vec3(-0.0f, -0.5f, 0.5f));
		dirLight.color = glm::vec3(0.75, 0.75, 0.94);
		dirLight.projection = glm::ortho(-80.0f, 80.0f, -80.0f, 80.0f, -100.0f, 100.0f);

		dirLights.push_back(dirLight); //save it to buffer

									   //TEMP LIGHT INIT:

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);

		const int LIGHT_RADIUS = 30; //Radius of lights

		if (lightBuffer == 0) {
			return;
		}

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind light buffer
		//Lights::PointLight *pointLightsPtr = (Lights::PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE); //get pointer of the data in the buffer

		//for (int i = 0; i < NUM_LIGHTS; i++) {
		//	Lights::PointLight &light = pointLightsPtr[i]; //get light at pos i

		//	glm::vec3 position = glm::vec3(0.0);
		//	for (int i = 0; i < 3; i++) { // calculate random pos for light
		//		float min = LIGHT_MIN_BOUNDS[i];
		//		float max = LIGHT_MAX_BOUNDS[i];
		//		position[i] = (GLfloat)dis(gen) * (max - min) + min;
		//	}

		//	light.pos = glm::vec4(position, 1);
		//	light.color = glm::vec4(dis(gen), dis(gen), dis(gen), 1); //give the light a random color between 0 and 1
		//	light.radius.x = LIGHT_RADIUS;														  //DISCO
		//															  /*color[i] = glm::vec3(light.color);
		//															  light.radius.z = LIGHT_RADIUS;*/
		//}

		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); //close buffer
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void GearEngine::shaderInit()
	{
		
		glGenTextures(1, &gloomTexture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gloomTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)WINDOW_WIDTH, (GLsizei)WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

	}

	void GearEngine::skyboxInit()
	{
		skybox->init();

		std::vector<const GLchar*> faces;
		faces.push_back("skybox/right.dds");
		faces.push_back("skybox/left.dds");
		faces.push_back("skybox/top.dds");
		faces.push_back("skybox/bottom.dds");
		faces.push_back("skybox/front.dds");
		faces.push_back("skybox/back.dds");
		skybox->loadCubemap(faces);
	}
#pragma endregion
#pragma region bluh
	GEAR_API void GearEngine::setDrawMode(int drawMode)
	{
		this->drawMode = drawMode;
	}

	void GearEngine::drawQuad()
	{
		glDepthMask(GL_FALSE);
		if (quadVAO == 0) { //just draws a quad on the screen
			GLfloat quadVertices[] = {
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};

			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		}

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
	}

	void GearEngine::bindAnimations(Animation** theAnims, int* n)
	{
		animationCount = n;
		allAnims = theAnims;
	}

	void GearEngine::setFont(FontAsset* font)
	{
		text.setFont(font);
	}

	void GearEngine::setWorkQueue( WorkQueue* workQueue )
	{
		work = workQueue;
		queue.setWorkQueue( workQueue );
	}

	GEAR_API Skybox * GearEngine::getSkybox()
	{
		return this->skybox;
	}

	void GearEngine::print(const std::string &s, const float &baseX, const float &baseY, const float &scale, const glm::vec4 &color)
	{
		
		text.print(s, baseX, baseY, scale, color);
	}

	void GearEngine::showImage(const sScreenImage & quad, Importer::TextureAsset* texture)
	{
		image.showImage(quad, texture);
	}

	void GearEngine::showImage(const glm::vec2 &pos, const float &width, const float &height, Importer::TextureAsset* texture)
	{
		image.showImage(pos, width, height, texture);
	}

	GEAR_API void GearEngine::showWorldImage(const sWorldImage & quad, Importer::TextureAsset * texture)
	{
		worldImage.showImage(quad, texture);
	}

	GEAR_API void GearEngine::showWorldImage(const glm::vec3 & pos, const float & width, const float & height, Importer::TextureAsset * texture)
	{
		worldImage.showImage(pos, width, height, texture);
	}

	glm::vec2 GearEngine::getTextDimensions( const char* t )
	{
		return text.getTextDimensions( t );
	}
#pragma region queue functions
	void GearEngine::queueModels(std::vector<ModelInstance>* models)
	{
		staticModels = models;
	}

	void GearEngine::queueDynamicModels(std::vector<ModelInstance>* models)
	{
		dynamicModels = models;

		//updateTransforms( dynamicModels );

		//for (auto &m : *dynamicModels)
			//m.allocateBuffer();
	}

	void GearEngine::queueAnimModels(std::vector<ModelInstance>* models)
	{
		animatedModels = models;
	}

	void GearEngine::queueForwardModels(std::vector<ModelInstance>* models)
	{
		forwardModels = models;
	}

	void GearEngine::queueParticles(std::vector<Gear::ParticleSystem*> &ps)
	{
		particleSystem = &ps;
	}

	GEAR_API void GearEngine::queueEmitters(std::vector<Gear::ParticleEmitter*> &emitters)
	{
		particleEmitters = &emitters;
	}

	void GearEngine::queueLights(std::vector<Lights::PointLight>* lights)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind light buffer
		Lights::PointLight *pointLightsPtr = (Lights::PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE); //get pointer of the data in the buffer
		Lights::PointLight* light;
		for (int i = 0; i < lights->size(); i++) {
			if (i < NUM_LIGHTS)
			{
				light = &pointLightsPtr[i]; //get light at pos i

				light->pos = lights->at(i).pos;
				light->color = lights->at(i).color;
				light->radius = lights->at(i).radius;
				light->radius.a = (float)i;
			}
			else {
				printf("ERROR: Too many lights : " + lights->size());
			}
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); //close buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void GearEngine::queueLights(Lights::DirLight * lights)
	{
		this->dirLights.at(0).color = lights->color;
		this->dirLights.at(0).direction = lights->direction;
	}
	GEAR_API void GearEngine::queueAddLights(Lights::PointLight * lights)
	{
		this->addLightQueue.push_back(lights);
	}
	GEAR_API void GearEngine::queueUpdateLights(Lights::PointLight * lights)
	{
		this->updateLightQueue.push_back(lights);
	}
	GEAR_API void GearEngine::queueRemoveLights(Lights::PointLight * lights)
	{
		this->removeLightQueue.push_back(lights);
	}

	GEAR_API void GearEngine::queueTextureBlendings(std::vector<ModelInstance>* blendingModels)
	{
		blendModels = blendingModels;
	}

#pragma endregion
	
	void GearEngine::draw(Camera* camera)
	{		
		//queue.update(*transformCount, *allTrans);
		Camera tempCamera;

		shadow.calcOrthoProjs(camera);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);	
		
		for (int i = 0; i < shadow.getNumCascades(); i++)
		{
			shadow.bind(i);
			ShaderProgram *shader = queue.getShaderProgram(ShaderType::GEOMETRYSHADOW);
			shader->use();
			shader->setUniform(shadow.getShadowMatrix()[i], "ViewProjectionMatrix");
			shader->unUse();

			shader = queue.getShaderProgram(ShaderType::ANIMSHADOW);
			shader->use();
			shader->setUniform(shadow.getShadowMatrix()[i], "ViewProjectionMatrix");
			shader->unUse();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			queue.geometryPass(dynamicModels, animatedModels, dirLights[0]);
			shadow.unBind();
		}

		//shadowMap.use();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//queue.geometryPass(dynamicModels, animatedModels, dirLights[0]); // renders the geometry into the gbuffer
		//shadowMap.unUse();
		//shadowMapBlur(&shadowMapTemp, &shadowMap, 0.9f);

		queue.updateUniforms(camera);

		gBuffer.use();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		queue.geometryPass(dynamicModels, animatedModels); // renders the geometry into the gbuffer
		gBuffer.unUse();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.getFramebufferID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		

		glDisable(GL_CULL_FACE);

		lightPass(camera, &tempCamera); //renders the texture with light calculations

		

		debugHandler->draw( camera, &queue );

		skybox->update(camera);
		skybox->draw();

		queue.particlePass(particleSystem, particleEmitters);
		
		queue.textureBlendingPass(&textureBlend, blendModels);

		queue.forwardPass(forwardModels, &uniValues);

		worldImage.update(camera);
		worldImage.draw();

		staticModels = &defaultModelList;
		dynamicModels = &defaultModelList;

		
		image.draw();
		text.draw();

		//ShaderProgram *shader = queue.getShaderProgram(ShaderType::QUAD);
		//for (int i = 0; i < shadow.getNumCascades(); i++)
		//{
		//	glViewport(10 + 200 * i + 10 * i, WINDOW_HEIGHT - 210, 200, 200);
		//	shader->use();
		//	shadow.bindTexture(shader, "diffuse", 0, i);
		//	drawQuad(); //draws quad
		//	shader->unUse();
		//}


		//glViewport(220, 10, 200, 200);
		//shader->use();
		//shadow.bindTexture(shader, "texture", 0, 1);
		//drawQuad(); //draws quad
		//shader->unUse();

		//glViewport(430, 10, 200, 200);
		//shader->use();
		//shadow.bindTexture(shader, "texture", 0, 2);
		//drawQuad(); //draws quad
		//shader->unUse();

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	}

	void GearEngine::update(float dt)
	{
		queue.update(*animationCount, *allAnims);
		debugHandler->update();
		debugHandler->reset();
		text.updateBuffer();
		image.updateBuffer();
		worldImage.updateBuffer();
		addLight();
		updateLight();
		removeLight();
		addDynamicLight();
		updateDynamicLight();
		removeDynamicLight();
		skybox->updateRotation(dt);
		skybox->updateFog(dt);
		updateTransforms( dynamicModels );
		updateTransforms( animatedModels );
		updateTransforms( forwardModels );
		updateTransforms( blendModels );
	}

	void GearEngine::updateTransforms( std::vector<ModelInstance>* models )
	{
		//for( int curModel=0; curModel<models->size(); curModel++ )
		//{
		//	glm::mat4 ident;
		//	for( int curTrans=0; curTrans < models->at(curModel).transforms.size(); curTrans++ )
		//	{
		//		TransformStruct& t = models->at(curModel).transforms.at(curTrans);
		//		glm::vec3 tempLook = glm::normalize(glm::vec3(t.lookAt.x, 0, t.lookAt.z));
		//		glm::vec3 axis = glm::cross(tempLook, { 0, 1, 0 });
		//
		//		glm::mat4 tempMatrix = glm::translate( ident, t.pos );
		//		tempMatrix = glm::scale( tempMatrix, t.scale );
		//		tempMatrix = glm::rotate( tempMatrix, t.rot.z, axis );
		//		tempMatrix = glm::rotate( tempMatrix, t.rot.y, { 0, 1, 0 } );
		//
		//		models->at(curModel).worldMatrices.at(curTrans) = tempMatrix;
		//	}
		//}

		for (auto &m : *models)
		{
			m.updateWorldMatrices();
			m.allocateBuffer();
		}
	}

	GEAR_API void GearEngine::addLight()
	{
		if (addLightQueue.size() > 0)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind light buffer
			Lights::PointLight *pointLightsPtr = (Lights::PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE); //get pointer of the data in the buffer
			Lights::PointLight *light;
			for (int j = 0; j < addLightQueue.size(); j++)
			{
				for (int i = 0; i < NUM_LIGHTS; i++) {
					light = &pointLightsPtr[i]; //get light at pos i
					if (light->radius.a == -1)
					{
						light->pos = addLightQueue[j]->pos;
						light->color = addLightQueue[j]->color;
						light->radius = addLightQueue[j]->radius;
						light->radius.a = (float)i;
						addLightQueue[j]->radius.a = (float)i;
						i = NUM_LIGHTS;
					}
				}
			}
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); //close buffer
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			addLightQueue.clear();
		}
	}

	GEAR_API void GearEngine::updateLight()
	{
		//if (updateLightQueue.size() > 0)
		//{
		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind light buffer
		//	Lights::PointLight *pointLightsPtr = (Lights::PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); //get pointer of the data in the buffer
		//	/*for (int j = 0; j < updateLightQueue.size(); j++)
		//	{
		//		if ((int)updateLightQueue[j]->radius.a >= 0)
		//		{
		//			Lights::PointLight &light = pointLightsPtr[(int)updateLightQueue[j]->radius.a];
		//			light.pos = updateLightQueue[j]->pos;
		//			light.color = updateLightQueue[j]->color;
		//			light.radius = updateLightQueue[j]->radius;
		//		}
		//	}*/
		//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); //close buffer
		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		//	//updateLightQueue.clear();
		//}
	}

	GEAR_API void GearEngine::removeLight()
	{
		if (removeLightQueue.size() > 0)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind light buffer
			Lights::PointLight *pointLightsPtr = (Lights::PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE); //get pointer of the data in the buffer
			for (int j = 0; j < removeLightQueue.size(); j++)
			{
				for (int i = 0; i < NUM_LIGHTS; i++) {
					if (i == removeLightQueue[j]->radius.a)
					{
						Lights::PointLight &light = pointLightsPtr[i]; //get light at pos i

						light.pos = { 0,0,0,0 };
						light.color = { 0,0,0,0 };
						light.radius = { 0,0,0,-1 };
					}
				}
			}

			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); //close buffer
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			removeLightQueue.clear();
		}
	}

	GEAR_API void GearEngine::queueAddDynamicLights(Lights::PointLight * lights)
	{
		addDynamicLightQueue.push_back(lights);
	}

	GEAR_API void GearEngine::queueUpdateDynamicLights(Lights::PointLight * lights)
	{
		updateDynamicLightQueue.push_back(lights);
	}

	GEAR_API void GearEngine::queueRemoveDynamicLights(Lights::PointLight * lights)
	{
		removeDynamicLightQueue.push_back(lights);
	}

	GEAR_API void GearEngine::addDynamicLight()
	{
		if (addDynamicLightQueue.size() > 0 && dynamicPointlights.size() < NUM_DYNAMIC_LIGHTS)
		{
			for (int j = 0; j < addDynamicLightQueue.size(); j++)
			{
				addDynamicLightQueue[j]->radius.a = dynamicPointlights.size();
				dynamicPointlights.push_back(addDynamicLightQueue[j]);
			}

			addDynamicLightQueue.clear();
		}
	}

	GEAR_API void GearEngine::updateDynamicLight()
	{
		if (updateDynamicLightQueue.size() > 0)
		{
			for (int j = 0; j < updateDynamicLightQueue.size(); j++)
			{
				dynamicPointlights[updateDynamicLightQueue[j]->radius.a] = updateDynamicLightQueue[j];
			}
			updateDynamicLightQueue.clear();
		}
	}

	GEAR_API void GearEngine::removeDynamicLight()
	{
		if (removeDynamicLightQueue.size() > 0)
		{
			for (size_t k = 0; k < removeDynamicLightQueue.size(); k++)
			{
				for (size_t i = 0; i < dynamicPointlights.size();)
				{
					if (dynamicPointlights.at(i) == removeDynamicLightQueue.at(k)) {

						dynamicPointlights.erase(dynamicPointlights.begin() + i);
					}
					else
						i++;
				}
			}
			removeDynamicLightQueue.clear();
		}
	}

	GEAR_API void GearEngine::resetLightbuffer()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer); //bind light buffer
		Lights::PointLight *pointLightsPtr = (Lights::PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE); //get pointer of the data in the buffer

		for (int i = 0; i < NUM_LIGHTS; i++) {
			Lights::PointLight &light = pointLightsPtr[i]; //get light at pos i

			light.pos = {0,0,0,0};
			light.color = { 0,0,0,0 };
			light.radius = { 0,0,0,-1 };
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); //close buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void GearEngine::pickingPass() {
		gBuffer.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		queue.pickingPass(dynamicModels);
		
		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[3];
		glReadPixels(1024 / 2, 768 / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

		int pickedID = data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;
		std::cout << "Color: R: " << (int)data[0] << " | G: " << (int)data[1] << " | B: " << (int)data[2] << std::endl;
		//if (pickedID == 0x00000000) {
		//	std::cout << "looking at background!" << std::endl;
		//}
		//else
		//	std::cout << "Looking at something :): " << pickedID << std::endl;
		gBuffer.unUse();
	}
#pragma endregion

	void GearEngine::lightPass(Camera * camera, Camera* tempCam)
	{
		ShaderProgram *shader = queue.getShaderProgram(ShaderType::LIGHT_PASS);
		shader->use();
		glClear(GL_COLOR_BUFFER_BIT);
		gBuffer.BindTexturesToProgram(shader, "gAlbedoSpec", 0, 0); //binds textures
		gBuffer.BindTexturesToProgram(shader, "gNormal", 1, 1);
		gBuffer.BindTexturesToProgram(shader, "gDepth", 2, 2);

		for (GLuint i = 0; i < shadow.getNumCascades(); i++)
		{
			shadow.bindTexture(shader, ("gShadowMap[" + std::to_string(i) + "]").c_str(), 3 + i, i);

			glm::vec4 View = { 0.0f, 0.0f, -shadow.getSplitDistance()[i], 1.0f };
			glm::vec4 Clip = camera->getProjectionMatrix() * View;

			Clip.z /= Clip.w;

			shader->setUniform((shadow.getShadowMatrix()[i]), ("lightWVP[" + std::to_string(i) + "]").c_str());
			shader->setUniform((Clip.z), ("CascadeEndClipSpace[" + std::to_string(i) + "]").c_str());
		}

		shader->setUniform(camera->getPosition(), "viewPos"); // viewPos
		//shader->setUniform(shadow.projectionMatrices[1] * shadow.viewMatrices[1], "shadowVPM"); //shadowVPM
		shader->setUniform(drawMode, "drawMode"); //sets the draw mode to show diffrent lights calculations and textures for debugging  
		shader->setUniform(glm::inverse(camera->getViewMatrix()), "invView"); // invView
		shader->setUniform(glm::inverse(camera->getProjectionMatrix()), "invProj"); // invProj
		shader->setUniform(skybox->getFogColor(), "fogColor");

		for (GLuint i = 0; i < dirLights.size(); i++)
		{
			shader->setUniform(dirLights[i].direction, "dirLights.direction");
			shader->setUniform(dirLights[i].color, "dirLights.color");
		}
		int num_lights = dynamicPointlights.size();
		shader->setUniform(num_lights, "num_dynamic_lights");
		for (int i = 0; i < dynamicPointlights.size(); i++)
		{
			shader->setUniform(dynamicPointlights[i]->pos, "dynamicLights[" + std::to_string(i) + "].pos");
			shader->setUniform(dynamicPointlights[i]->color, "dynamicLights[" + std::to_string(i) + "].color");
			shader->setUniform(dynamicPointlights[i]->radius, "dynamicLights[" + std::to_string(i) + "].radius");
		}

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightBuffer); //binds the light buffer to the shader
		drawQuad(); //draws quad

		shader->unUse();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	}

	void GearEngine::BlurFilter(ShaderProgram * dest, ShaderProgram * source, glm::vec3 blurScale)
	{
		ShaderProgram *shader = queue.getShaderProgram(ShaderType::LIGHT_PASS);
		dest->use();
		glClear(GL_DEPTH_BUFFER_BIT);

		shader->use();

		shader->setUniform(blurScale, "blurScale");
		source->BindTexturesToProgram(shader, "filterTexture", 0, 0);
		drawQuad();

		shader->unUse();
	}

	void GearEngine::shadowMapBlur(ShaderProgram * dest, ShaderProgram * source, float blurAmount)
	{
		glm::vec3 blurScale = glm::vec3(1.0 / (blurAmount * source->getWidth()), 0, 0);
		BlurFilter(dest, source, blurScale);

		blurScale = glm::vec3(0, 1.0 / (blurAmount * source->getHeight()), 0);
		BlurFilter(source, dest, blurScale);
	}

	void GearEngine::frameBufferInit()
	{
		GLuint internalFormat[] = { GL_RGBA,GL_RGB16F, GL_R32F }; //Format for texture in gBuffer
		GLuint format[] = { GL_RGBA,GL_RGB, GL_RED }; //Format for texture in gBuffer
		GLuint attachment[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; //gBuffer attachements
		GLenum type[] = { GL_UNSIGNED_INT, GL_FLOAT, GL_FLOAT }; //data type for texture
		GLfloat filter[] = { GL_NEAREST, GL_NEAREST, GL_NEAREST};

		gBuffer.initFramebuffer(3, WINDOW_WIDTH, WINDOW_HEIGHT, filter, internalFormat, format, type, attachment, false);

		/*shadowMap.initFramebuffer(1, WINDOW_HEIGHT, WINDOW_HEIGHT, GL_LINEAR, GL_RG32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, true);
		shadowMapTemp.initFramebuffer(1, WINDOW_HEIGHT, WINDOW_HEIGHT, GL_LINEAR, GL_RG32F, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, true);*/
		//particleFBO.initFramebuffer(1, WINDOW_WIDTH, WINDOW_HEIGHT, GL_LINEAR, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, false);
	}

	void GearEngine::addDebugger(Debug* debugger) {
		debugHandler->addDebuger(debugger);
	}
}