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
#include "Particles.h"
#include "Controls.h"
#include <lua\lua.hpp>
#include "LuaBinds.h"
#include <String>
#include <thread>
#include "HeightMap.h"
#include "Ray.h"

void allocateTransforms(int n);

Window *window = new Window();
Gear::GearEngine *engine = new Gear::GearEngine();

int startNetworkCommunication();
int startNetworkSending(Nurn::NurnEngine * pSocket);
int startNetworkReceiving(Nurn::NurnEngine * pSocket);

std::thread networkThread;
bool networkActive = false;
bool networkHost = true;
int port = 30000;
const Nurn::Address networkAddress = Nurn::Address(127, 0, 0, 1, port);

bool running = true;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Importer::Assets assets = *Importer::Assets::getInstance();
	Importer::ModelAsset* molebat = assets.load<Importer::ModelAsset>( "Models/moleRat.mtf" );
	Importer::ModelAsset* box = assets.load<Importer::ModelAsset>( "Models/mesh.mtf" );
	Importer::TextureAsset* redTexture = assets.load<Importer::TextureAsset>( "Textures/molerat_texturemap2.png" );
	Importer::TextureAsset* greenTexture = assets.load<Importer::TextureAsset>( "Textures/green.dds" );
	Importer::ImageAsset* heightMapAsset = assets.load<Importer::ImageAsset>("Textures/molerat_texturemap4.png");
	HeightMap *heightMap = new HeightMap();
	
	heightMap->loadHeightMap(heightMapAsset, false);
	
	redTexture->bind();
	std::vector<ModelInstance> models;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	transformReg(L);
	if (luaL_dofile(L, "Scripts/test.lua"))
	{
		std::cout<<("%s\n", lua_tostring(L, -1)) << "\n";
	}
	
	//for( int i=0; i<nrOfTransforms; i++ )

	/*skybox.setModelAsset(skyboxAsset);
	skybox.worldMatrix[3][1] = 3;
	*/
	//allocateTransforms(nrOfTransforms);
	for( int i=0; i<3; i++ )
		engine->renderQueue.addModelInstance(molebat);
	
	Gear::Particle particle;
	glm::vec3 pos;
	glm::vec3 color;

	controls.setControl(&allTransforms[0]);
	//player.weperino.fml = &engine->renderElements;
	
	/*for (int i = 0; i < 100; i++) {
		player.weperino.magics[i].transform = &allTransforms[engine->renderQueue.addModelInstance(molebat)];
	}*/
	controls.setControl(&allTransforms[2]);

	for (int i = 0; i < particle.getParticleCount(); i++)
	{
		pos = {rand() % 10, rand() % 5, rand() % 10 };
		color = {1.0, 0.0, 0.0};
		particle.setParticle(pos, color, i);
		particle.getParticle();
		engine->renderQueue.particles.push_back( &particle );
	}
	glEnable( GL_DEPTH_TEST );
	
	GLFWwindow* w = window->getGlfwWindow();
	Inputs inputs(w);

	PerformanceCounter counter;
	double frameTime = 0.0;
	int frameCounter = 0;

	Camera camera(45.f, 1280.f/720.f, 0.1f, 2000.f, &inputs);


	bool running = true;
	float* transforms = new float[6 * nrOfTransforms];
	glm::vec3* lookAts = new glm::vec3[nrOfTransforms];
	if (networkActive)
	{
		networkThread = std::thread(startNetworkCommunication);
	}

	while (running && window->isWindowOpen())
	{
		deltaTime = counter.getDeltaTime();

		inputs.update();
		controls.sendControls(inputs, L);
		//player.update(&inputs, deltaTime);
		particle.setParticle(allTransforms[2].getPos(), glm::vec3(1,0,0), 0 );
		camera.follow(controls.getControl()->getPos(), controls.getControl()->getLookAt(), abs(inputs.getScroll())+5.f);

		for (int i = 0; i < nrOfTransforms; i++) {
			transforms[i * 6] = allTransforms[i].getPos().x;
			transforms[i * 6 + 1] = allTransforms[i].getPos().y;
			transforms[i * 6 + 2] = allTransforms[i].getPos().z;
			transforms[i * 6 + 3] = allTransforms[i].getRotation().x;
			transforms[i * 6 + 4] = allTransforms[i].getRotation().y;
			transforms[i * 6 + 5] = allTransforms[i].getRotation().z;
		}

		for (int i = 0; i < nrOfTransforms; i++)
		{
			lookAts[i] = allTransforms[i].getLookAt();
		}
		engine->renderQueue.update(transforms, nullptr, nrOfTransforms, lookAts);
		

		engine->draw(&camera);
		window->update();	

		if( inputs.keyPressed( GLFW_KEY_ESCAPE ) )
			running = false;
		if( inputs.keyPressedThisFrame( GLFW_KEY_1 ) )
			redTexture->bind();
		else if( inputs.keyPressedThisFrame( GLFW_KEY_2 ) )
			greenTexture->bind();

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
	}
	delete[] transforms;
	delete[] lookAts;
	delete heightMap;

	if (networkActive)
	{
		networkThread.join();
	}

	delete[] allTransforms;
	lua_close(L);
	//delete window;
	glfwTerminate();
	delete engine;
	return 0;
}

void allocateTransforms(int n)
{
	if(allTransforms!= nullptr)
		delete allTransforms;
	allTransforms = new Transform[n];
	engine->renderQueue.allocateWorlds(n);
}


int startNetworkCommunication()
{
	// initialize socket layer

	Nurn::NurnEngine socket;

	if (!socket.InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	// create socket

	printf("creating socket on port %d\n", port);

	if (!socket.CreateUDPSocket(port))
	{
		printf("failed to create socket!\n");
		return 1;
	}



	if (networkHost)
	{
		startNetworkReceiving(&socket);
	}
	else
	{
		startNetworkSending(&socket);
	}

	printf("Closing socket on port %d\n", port);
	socket.ShutdownSockets();

	return 0;
}

int startNetworkSending(Nurn::NurnEngine * pSocket)
{
	while (running && window->isWindowOpen())
	{
		const char data[] = "hello world!";

		pSocket->Send(networkAddress, data, sizeof(data));

		Sleep(250);
	}

	return 0;
}

int startNetworkReceiving(Nurn::NurnEngine * pSocket)
{
	while (running && window->isWindowOpen())
	{
		Sleep(250);
		Nurn::Address sender;
		unsigned char buffer[256];
		int bytes_read = pSocket->Receive(sender, buffer, sizeof(buffer));
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