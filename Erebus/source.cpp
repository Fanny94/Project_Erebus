#include <iostream>
#include "Nurn.hpp"
#include "Gear.h"
#include "Inputs.h"
#include "Assets.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Window.h"
#include "Transform.h"
#include "PerformanceCounter.h"
#include "ParticleSystem.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "CollisionHandler.h"
#include "Controls.h"
#include "LuaBinds.h"
#include <String>
#include <thread>
#include "HeightMap.h"
#include "Ray.h"
#include "FontAsset.h"
//#include "LevelEditor.h"

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
	Window window;
	Gear::GearEngine engine;

	Importer::Assets assets;
	Importer::FontAsset* font = assets.load<FontAsset>( "Fonts/System" );
	engine.setFont(font);
	int nrOfTransforms = 100;
	int boundTransforms = 0;
	Transform* transforms = new Transform[nrOfTransforms];
	TransformStruct* allTransforms = new TransformStruct[nrOfTransforms];
	for (int i = 0; i < nrOfTransforms; i++)
		transforms[i].setThePtr(&allTransforms[i]);
	Controls controls;
	engine.allocateWorlds(nrOfTransforms);

	Importer::ModelAsset* moleman = assets.load<ModelAsset>( "Models/moleman5.model" );

	std::vector<ModelInstance> models;
	std::vector<AnimatedInstance> animatedModels;

	CollisionHandler collisionHandler;
	collisionHandler.setTransforms(transforms);

	std::vector<Gear::ParticleSystem*> ps;
	glEnable(GL_DEPTH_TEST);

	GLFWwindow* w = window.getGlfwWindow();
	Inputs inputs(w);
	
	window.changeCursorStatus(false);

	Camera camera(45.f, 1280.f / 720.f, 0.1f, 2000.f, &inputs);

	engine.bindTransforms(&allTransforms, &boundTransforms);
	if (networkActive)
	{
		networkThread = std::thread(startNetworkCommunication, &window );
	}

	LuaBinds luaBinds;
	luaBinds.load( &engine, &assets, &collisionHandler, &controls, transforms, &boundTransforms, &models, &animatedModels, &camera, &ps);

	PerformanceCounter counter;
	double deltaTime;
	bool lockMouse = false;
	while (running && window.isWindowOpen())
	{	
		deltaTime = counter.getDeltaTime();
		inputs.update();
		controls.update(&inputs);
		luaBinds.update( &controls, deltaTime);
		
		for (int i = 0; i < ps.size(); i++)
			ps.at(i)->update(deltaTime);
		
		engine.queueDynamicModels(&models);
		engine.queueAnimModels(&animatedModels);
		engine.queueParticles(&ps);

		collisionHandler.checkCollisions();


		engine.draw(&camera);

		if( inputs.keyPressed( GLFW_KEY_ESCAPE ) )
			running = false;
		
		if (inputs.keyPressedThisFrame(GLFW_KEY_J))
			engine.setDrawMode(1);
		else if( inputs.keyPressedThisFrame( GLFW_KEY_K ))
			engine.setDrawMode(2);
		else if (inputs.keyPressedThisFrame(GLFW_KEY_L))
			engine.setDrawMode(3);
		else if (inputs.keyPressedThisFrame(GLFW_KEY_P))
			engine.setDrawMode(4);
		else if (inputs.keyPressedThisFrame(GLFW_KEY_N))
			engine.setDrawMode(5);
		else if (inputs.keyPressedThisFrame(GLFW_KEY_O))
			engine.setDrawMode(6);
		else if (inputs.keyPressedThisFrame(GLFW_KEY_R))
		{
			if (lockMouse)
			{
				window.changeCursorStatus(false);
				lockMouse = false;
			}
			else
			{
				window.changeCursorStatus(true);
				lockMouse = true;
			}
		}


		window.update();
		std::string fps = "FPS: " + std::to_string(counter.getFPS());
		engine.print(fps, 0.f, 720.f);
	}

	luaBinds.unload();
	delete[] allTransforms;
	delete[] transforms;
	if (networkActive)
	{
		networkThread.join();
	}
	for (int i = 0; i < ps.size(); i++)
		delete ps.at(i);

	glfwTerminate();
	return 0;
}

int startNetworkCommunication( Window* window )
{
	// initialize socket layer

	Nurn::NurnEngine network;
	Nurn::NurnEngine network2;

	if (networkHost)
	{
		if (!network.InitializeHost())
		{
			printf("failed to initialize sockets\n");
			return 1;
		}

		Sleep(250);

		if (!network2.InitializeClient(127, 0, 0, 1, 35500, 35501))
		{
			printf("failed to initialize sockets\n");
			return 1;
		}

		if (!network.AcceptCommunication())
		{
			printf("failed to accept connection\n");
			return 1;
		}

		while (running && window->isWindowOpen())
		{
			startNetworkSending(&network2, window);
			startNetworkReceiving(&network, window);
		}
	}
	else
	{
		if (!network.InitializeClient(127,0,0,1,35501))
		{
			printf("failed to initialize sockets\n");
			return 1;
		}
		startNetworkSending(&network, window);
	}

	printf("Closing socket on port\n");
	network.Shutdown();
	network2.Shutdown();

	return 0;
}

int startNetworkSending(Nurn::NurnEngine * pNetwork, Window* window)
{
	const char data[] = "hello world!";

	pNetwork->Send(data, sizeof(data));

	Sleep(250);

	return 0;
}

int startNetworkReceiving(Nurn::NurnEngine * pNetwork, Window* window)
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

	return 0;
}
