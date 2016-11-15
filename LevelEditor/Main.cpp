#include<AntTweakBar.h>


#include"Importer.h"
#include"MenuTest.h"
#include "Gear.h"
#include "BaseIncludes.h"

#include"Window.h"
#include"HeightMapEditor.h"
#include"ParticleEditor.h"

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) { TwEventMouseButtonGLFW(button, action); }

inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) { TwMouseMotion(int(xpos), int(ypos)); }

enum EditorState
{
	HEIGHTMAP_EDIT,
	PARTICES_EDIT,

};

int main()
{
	EditorState editorState;
	editorState = HEIGHTMAP_EDIT;

	Window * window = new Window();

	Gear::GearEngine *engine = new Gear::GearEngine();

	Importer::ModelImporter* importer = new Importer::ModelImporter();
	importer->load("Models/newmolebat.mole");

	// TEMPORARY: F�r att kunna rita ut modellen
	engine->vbo = importer->getVertexBuffer(0);
	engine->size = importer->getBufferSize(0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(45.f, 1280.f / 720.f, 0.1f, 20.f);

	TwInit(TW_OPENGL_CORE, NULL);

	MenuTest menuTest;
	ParticleEditor* particleEditor;
	HeightMapEditor* heightMapEditor;
	
	

	TwWindowSize(1280, 720);
	
	glfwSetMouseButtonCallback(window->getGlfwWindow(),(GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window->getGlfwWindow(), (GLFWcursorposfun)TwEventMousePosGLFW3);
	
	
	
	while (engine->isRunning())
	{

		
		switch (editorState)
		{
		case HEIGHTMAP_EDIT:

			break;

		case PARTICES_EDIT:

			break;
		default:
			break;
		}

	#pragma region DRAW
		glfwPollEvents();
		engine->draw(&camera);
		TwDraw();
		window->update();

	#pragma endregion
	
	}

	return 0;
}
