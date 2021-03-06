#include "Menu.h"

Menu::Menu(Gear::GearEngine* inEngine, Importer::Assets* assets)
{
	engine = inEngine;

	background = sScreenImage(glm::vec2(0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);
	menuBackground = assets->load<TextureAsset>("Textures/menuBackground.dds");

	button = sScreenImage(glm::vec2(350, 300), 350, 100);
	buttonTex = assets->load<TextureAsset>("Textures/button.dds");

	hostButton = sScreenImage(glm::vec2(350, 425), 350, 100);
	hostButtonTex = assets->load<TextureAsset>("Textures/buttonHost.dds");
	clientButton = sScreenImage(glm::vec2(350, 550), 350, 100);
	clientButtonTex = assets->load<TextureAsset>("Textures/buttonConnect.dds");


}

Menu::~Menu()
{

}

enum GameState Menu::Update(Inputs* inputs)
{

	if (button.mousePick((float)inputs->getMousePos().x, (float)inputs->getMousePos().y) && inputs->buttonReleasedThisFrame(0))
	{

		return  GameplayState;
	}
	else if (hostButton.mousePick((float)inputs->getMousePos().x, (float)inputs->getMousePos().y) && inputs->buttonReleasedThisFrame(0))
	{

		return  HostGameplayState;
	}
	else if (clientButton.mousePick((float)inputs->getMousePos().x, (float)inputs->getMousePos().y) && inputs->buttonReleasedThisFrame(0))
	{

		return  ClientGameplayState;
	}

	return MenuState;
}

void Menu::Draw()
{
	engine->showImage(background, menuBackground);
	engine->showImage(button, buttonTex);
	engine->showImage(hostButton, hostButtonTex);
	engine->showImage(clientButton, clientButtonTex);
}