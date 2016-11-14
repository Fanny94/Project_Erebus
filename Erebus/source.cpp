#include <iostream>
#include "Gear.h"
#include "Importer.h"
#include <ctime>

void calculateDt(float& dt, const clock_t& start, const clock_t& end, const int& ticks);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Gear::GearEngine *engine = new Gear::GearEngine();
	Importer::ModelImporter* importer = new Importer::ModelImporter();
	importer->load( "Models/newmolebat.mole" );

	// TEMPORARY: F�r att kunna rita ut modellen
	engine->vbo = importer->getVertexBuffer( 0 );
	engine->size = importer->getBufferSize( 0 );

	glEnable( GL_DEPTH_TEST );
	
	clock_t c_start, c_end;
	float dt = 0, totalTicks = 0;
	float totalTime = 0;
	while (engine->isRunning()) {
		totalTicks++;
		c_start = clock();
		engine->draw();
		c_end = clock();
		calculateDt(dt, c_start, c_end, totalTicks);

	}

	delete importer;
	delete engine;
	//system("pause");
	return 0;
}

void calculateDt(float& dt, const clock_t& start, const clock_t& end, const int& ticks) {
	
	dt = ((float)end - (float)start) / CLOCKS_PER_SEC;
	//std::cout << dt << std::endl;
}