#include <iostream>
#include "Nurn.hpp"
#include "Gear.h"
#include "Inputs.h"
#include "Assets.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Window.h"
#include <ctime>
#include "Transform.h"
#include "PerformanceCounter.h"
#include "ParticleSystem.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "CollisionHandler.h"
#include "Controls.h"
#include <lua\lua.hpp>
#include "LuaBinds.h"
#include <String>
#include <thread>
#include "HeightMap.h"
#include "Ray.h"

int startNetworkCommunication( Window* window );
int startNetworkSending(Nurn::NurnEngine * pSocket, Window* window);
int startNetworkReceiving(Nurn::NurnEngine * pSocket, Window* window);

std::thread networkThread;
bool networkActive = false;
bool networkHost = true;

bool running = true;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Importer::Assets assets;
	int nrOfTransforms = 100;
	int boundTransforms = 0;
	Transform* transforms = new Transform[nrOfTransforms];
	Controls controls;

	Window window;
	Gear::GearEngine engine;
	engine.allocateWorlds(nrOfTransforms);

	std::vector<ModelInstance> models;
	engine.addDebugger(Debugger::getInstance());
	Debug* tempDebug = Debugger::getInstance();

	double deltaTime = 0.0;

	Importer::TextureAsset* moleratTexture = assets.load<Importer::TextureAsset>( "Textures/molerat_texturemap2.png" );
	moleratTexture->bind();

	CollisionHandler collisionHandler;
	collisionHandler.setTransforms( transforms );

	glEnable( GL_DEPTH_TEST );
	
	GLFWwindow* w = window.getGlfwWindow();
	Inputs inputs(w);

	PerformanceCounter counter;
	counter.startCounter();
	double frameTime = 0.0;
	int frameCounter = 0;

	Camera camera(45.f, 1280.f/720.f, 0.1f, 2000.f, &inputs);

	float* transformData = new float[6 * nrOfTransforms];
	glm::vec3* lookAts = new glm::vec3[nrOfTransforms];
	engine.bindTransforms(&transformData, nullptr, &boundTransforms, lookAts);

	if (networkActive)
	{
		networkThread = std::thread(startNetworkCommunication, &window );
	}

	LuaBinds luaBinds;
	luaBinds.load( &engine, &assets, &collisionHandler, &controls, transforms, &boundTransforms, &models );

	bool playerAlive = true;
	while (running && window.isWindowOpen())
	{
		deltaTime = counter.getDeltaTime();
		inputs.update();

		controls.update( &inputs );
		tempDebug->drawLine(glm::vec3(0, 10, 0), glm::vec3(100, 10, 100));
		luaBinds.update( &controls, deltaTime );

		camera.follow(controls.getControl()->getPos(), controls.getControl()->getLookAt(), abs(inputs.getScroll())+5.f);

		for (int i = 0; i < boundTransforms; i++) 
		{
			glm::vec3 pos = transforms[i].getPos();
			glm::vec3 rot = transforms[i].getRotation();
			glm::vec3 scale = transforms[i].getScale();
			transformData[i*9] = pos.x;
			transformData[i*9 + 1] = pos.y;
			transformData[i*9 + 2] = pos.z;
			transformData[i*9 + 3] = rot.x;
			transformData[i*9 + 4] = rot.y;
			transformData[i*9 + 5] = rot.z;
			transformData[i*9 + 6] = scale.x;
			transformData[i*9 + 7] = scale.y;
			transformData[i*9 + 8] = scale.z;

			lookAts[i] = transforms[i].getLookAt();
		}

		engine.draw(&camera, &models);
		window.update();	
		engine.queueDynamicModels(&models);
		engine.draw(&camera);

		if( inputs.keyPressed( GLFW_KEY_ESCAPE ) )
			running = false;

		//Display FPS:
		frameCounter++;
		frameTime += deltaTime;
		if (frameTime >= 1.0)
		{
			double fps = double(frameCounter) / frameTime;
			std::cout << "FPS: " << fps << std::endl;
			frameTime -= 1.0;
			frameCounter = 0;
		}

		//Collisions
		collisionHandler.checkCollisions();
	}

	luaBinds.unload();

	delete[] transforms;
	delete[] transformData;
	delete[] lookAts;

	if (networkActive)
	{
		networkThread.join();
	}

	glfwTerminate();
	return 0;
}

int startNetworkCommunication( Window* window )
{
	// initialize socket layer

	Nurn::NurnEngine network;

	if (!network.Initialize(127, 0, 0, 1))
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	if (networkHost)
	{
		startNetworkReceiving(&network, window);
	}
	else
	{
		startNetworkSending(&network, window);
	}

	printf("Closing socket on port\n");
	network.Shutdown();

	return 0;
}

int startNetworkSending(Nurn::NurnEngine * pNetwork, Window* window)
{
	while (running && window->isWindowOpen())
	{
		const char data[] = "hello world!";

		pNetwork->Send(data, sizeof(data));

		Sleep(250);
	}

	return 0;
}

int startNetworkReceiving(Nurn::NurnEngine * pNetwork, Window* window)
{
	while (running && window->isWindowOpen())
	{
		printf("Recieving package\n");
		Sleep(250);
		Nurn::Address sender;
		unsigned char buffer[256];
		int bytes_read = pNetwork->Receive(sender, buffer, sizeof(buffer));
		if (bytes_read)
		{
			printf("received packet from %d.%d.%d.%d:%d (%d bytes)\n",
				sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(),
				sender.GetPort(), bytes_read);
			std::cout << buffer << std::endl;
		}
	}

	return 0;
}