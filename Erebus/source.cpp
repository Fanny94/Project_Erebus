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
#include "Controls.h"
#include <lua\lua.hpp>
#include "LuaBinds.h"

Window *window = new Window();
Gear::GearEngine *engine = new Gear::GearEngine();

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Importer::Assets assets;
	Importer::ModelAsset* molebat = assets.load<Importer::ModelAsset>( "Models/moleRat.mtf" );
	Importer::ModelAsset* box = assets.load<Importer::ModelAsset>( "Models/mesh.mtf" );
	Importer::TextureAsset* redTexture = assets.load<Importer::TextureAsset>( "Textures/molerat_texturemap2.png" );
	Importer::TextureAsset* greenTexture = assets.load<Importer::TextureAsset>( "Textures/green.dds" );

	redTexture->bind();

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	transformReg(L);
	if (luaL_dofile(L, "Scripts/test.lua"))
	{
		std::cout<<("%s\n", lua_tostring(L, -1)) << "\n";
	}
	
	for( int i=0; i<nrOfTransforms; i++ )
		engine->renderQueue.addModelInstance(molebat);
	
	Gear::Particle particle;
	glm::vec3 pos;
	glm::vec3 color;

	controls.setControl(&allTransforms[0]);

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
	while (running && window->isWindowOpen())
	{
		deltaTime = counter.getDeltaTime();
		inputs.update();
		controls.sendControls(inputs, L);
		particle.setParticle(allTransforms[2].getPos(), glm::vec3(1,0,0), 0 );
		camera.follow(controls.getControl()->getPos(), controls.getControl()->getLookAt(), abs(inputs.getScroll())+5);		
		for (int i = 0; i < nrOfTransforms; i++) 
		{
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
		engine->renderQueue.update(transforms, nullptr, 50, lookAts);
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
	delete[] lookAts;
	delete[] transforms;
	delete[] allTransforms;
	lua_close(L);
	delete window;
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
