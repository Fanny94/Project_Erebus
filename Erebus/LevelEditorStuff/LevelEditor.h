#pragma once
#include <iostream>
#include "Nurn.hpp"
#include "Gear.h"
#include "Inputs.h"
#include "Assets.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Window.h"
#include <ctime>
#include "../Transform.h"
#include "../PerformanceCounter.h"
#include "../SphereCollider.h"
#include "../AABBCollider.h"
#include "../CollisionHandler.h"
#include "../Controls.h"
#include <String>
#include <thread>
#include "HeightMap.h"
#include "Ray.h"
#include "LevelTransformHandler.h"
#include "LevelModelHandler.h"
#include "LevelActorFactory.h"
#include "LevelAssetHandler.h"
#include "LevelUI.h"
#include "LevelActorHandler.h"
#include "LevelActionHandler.h"
#include "LevelGizmo.h"

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>
#include <d3d11_3.h>
#include <dxgi1_4.h>

#pragma comment(lib, "dxgi.lib")

class LevelEditor
{
private:
	enum actorTypes {
		PLAYER,
		STATIC,
		ENEMY,
		NR_ACTOR_TYPES
		
	};

	
	Gear::GearEngine* engine;
	LevelUI* ui;
	LevelActorFactory* factory;
	std::vector<Gear::ParticleSystem*> ps;
	LevelTransformHandler* transformHandler;
	LevelModelHandler* modelHandler;
	bool running;
	Camera* camera;
	Inputs* inputs;
	Window window;
	std::vector<LevelActor*> actors;
	int tempSelectedActorID;
	glm::vec3 tempSelectedHitPoint;
	//LevelGizmo* levelGizmo;

	void pick();
	bool holdingGizmo;
public:

	LevelEditor();
	~LevelEditor();

	void start();
	
};

