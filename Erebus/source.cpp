#include <iostream>
#include "Gear.h"
#include "Inputs.h"
#include "Assets.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Window.h"
#include <ctime>
#include "Transform.h"
#include "PerformanceCounter.h"
#include "Particles.h"
#include "Player.h"

void calculateDt(float& dt, const clock_t& start, const clock_t& end, const int& ticks);
void allocateTransforms(int n);

Window *window = new Window();
Transform* allTransforms;
Gear::GearEngine *engine = new Gear::GearEngine();


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//Importer::ModelAsset molebat;
	//molebat.load( "Models/mesh.mtf" );
	Importer::Assets assets = Importer::Assets::getInstance();
	Importer::ModelAsset* molebat = assets.load<Importer::ModelAsset>( "Models/moleRat.mtf" );
	Importer::TextureAsset* redTexture = assets.load<Importer::TextureAsset>( "Textures/molerat_texturemap2.png" );
	Importer::TextureAsset* greenTexture = assets.load<Importer::TextureAsset>( "Textures/green.dds" );
	//Importer::ModelAsset* skyboxAsset = assets.load<Importer::ModelAsset>("Models/skybox.mole");

	float skyboxScale = 1800;
	redTexture->bind();

	/*Gear::Model skybox;
	skybox.setModelAsset(skyboxAsset);
	skybox.worldMatrix[0][0] = skyboxScale;
	skybox.worldMatrix[1][1] = skyboxScale;
	skybox.worldMatrix[2][2] = skyboxScale;
	skybox.worldMatrix[3][3] = 1;

	skybox.worldMatrix[3][1] = 3;
	*/
	allocateTransforms(3);
	Model model;
	model.setModelAsset(molebat, engine->renderQueue.modelAdded(&model));
	model.setModelAsset(molebat, engine->renderQueue.modelAdded(&model));
	
	Gear::Particle particle;
	glm::vec3 pos;
	glm::vec3 color;


	Player player;
	Model playerModel;
	playerModel.setModelAsset(molebat, engine->renderQueue.modelAdded(&playerModel));
	player.renderqueue = &engine->renderQueue;
	player.weperino.fml = &engine->renderElements;
	playerModel.setModelAsset(molebat);

	player.model = &playerModel;



	// TEMP: Ritar ut modellen fr�n Gear.
	//engine->renderElements.push_back(&skybox);
	//engine->renderElements.push_back(player.model);


	for (int i = 0; i < particle.getParticleCount(); i++)
	{
		pos = {rand() % 10, rand() % 5, rand() % 10 };
		color = {1.0, 0.0, 0.0};

		particle.setParticle(pos, color, i);

		particle.getParticle();

		//engine->renderElements.push_back(&particle);
		engine->renderQueue.particles.push_back( &particle );
	}
	glEnable( GL_DEPTH_TEST );
	
	GLFWwindow* w = window->getGlfwWindow();
	Inputs inputs(w);
	clock_t c_start, c_end;
	float dt = 0;
	int totalTicks = 0;
	float totalTime = 0;
	totalTicks++;	

	PerformanceCounter counter;
	double frameTime = 0.0;
	int frameCounter = 0;
	Camera camera(45.f, 1280.f/720.f, 0.1f, 2000.f, &inputs);
	//glm::vec3 point = {0,0,5};
	glm::vec3 direction = {0,0,-1};

	float horizAngle = 3.14f;
	float vertAngle = 0;
	float speed = 8.f;

	bool freeCam = false;

	bool running = true;
	glm::vec3 point = {0,0,0};
	while (running && window->isWindowOpen()){
		c_start = clock();
		inputs.update();
		player.update(&inputs, dt);
		/*skybox.worldMatrix[3][0] = camera.getPosition().x;
		skybox.worldMatrix[3][1] = camera.getPosition().y- skyboxScale/2;
		skybox.worldMatrix[3][2] = camera.getPosition().z;*/
		camera.follow(player.position, player.lookAt, abs(inputs.getScroll())+5.f);
		//camera.camUpdate(point, direction, dt);
		
		float* transforms = new float[6];
		for (int i = 0; i < 2; i++) {
			transforms[i * 3] = allTransforms[i].getPos().x + i;
			transforms[i * 3 + 1] = allTransforms[i].getPos().y + i;
			transforms[i * 3 + 2] = allTransforms[i].getPos().z + i;
		}
		engine->renderQueue.update(transforms, nullptr, 2);
		delete[] transforms;
		
		engine->draw(&camera);
		window->update();
		c_end = clock();
		calculateDt(dt, c_start, c_end, totalTicks);

		frameCounter++;
		frameTime += counter.getDeltaTime();
		if (frameTime >= 1000.0)
		{
			double fps = double(frameCounter) / (frameTime / 1000.0);

			std::cout << "FPS: " << fps << std::endl;

			frameTime -= 1000.0;
			frameCounter = 0;
		}

		if( inputs.keyPressed( GLFW_KEY_ESCAPE ) )
			running = false;
		if( inputs.keyPressedThisFrame( GLFW_KEY_1 ) )
			redTexture->bind();
		else if( inputs.keyPressedThisFrame( GLFW_KEY_2 ) )
			greenTexture->bind();
	}
	delete[] allTransforms;
	delete window;
	glfwTerminate();
	delete engine;
	return 0;
}

void calculateDt(float& dt, const clock_t& start, const clock_t& end, const int& ticks) 
{	
	dt = ((float)end - (float)start) / CLOCKS_PER_SEC;
	//std::cout << dt << std::endl;
}

void allocateTransforms(int n)
{
	if(allTransforms!= nullptr)
		delete allTransforms;
	allTransforms = new Transform[n];
	engine->renderQueue.allocateWorlds(n);
}
