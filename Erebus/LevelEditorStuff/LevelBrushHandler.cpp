#include "LevelBrushHandler.h"
LevelBrushHandler* LevelBrushHandler::g_instance = nullptr;
LevelBrushHandler::~LevelBrushHandler()
{
}

void TW_CALL setSaveTypeCB(const void *value, void *s /*clientData*/)
{
	// Set: copy the value of s from AntTweakBar
	const std::string *srcPtr = static_cast<const std::string *>(value);
	std::string *dstPtr = static_cast<std::string *>(s);
	//TwCopyStdStringToLibrary(*dstPtr, *srcPtr); // the use of TwCopyStdStringToLibrary is required here
	*dstPtr = *srcPtr;
}

void TW_CALL getSaveTypeCB(void *value, void *s /*clientData*/)
{
	// Get: copy the value of s to AntTweakBar
	std::string *destPtr = static_cast<std::string *>(value);
	std::string *srcPtr = static_cast<std::string *>(s);
	TwCopyStdStringToLibrary(*destPtr, *srcPtr); // the use of TwCopyStdStringToLibrary is required here
}


void LevelBrushHandler::setTweakBar(TweakBar * brushBar)
{
	this->actionBar = brushBar;

	TwAddVarRW(actionBar->getBar(), "radius", TW_TYPE_FLOAT, &this->radius, NULL);
	TwAddVarRW(actionBar->getBar(), "density", TW_TYPE_FLOAT, &this->density, NULL);
	TwAddVarCB(actionBar->getBar(), "saveAsType", TW_TYPE_STDSTRING,setSaveTypeCB,getSaveTypeCB,&saveAsType,"");
}
void LevelBrushHandler::testDraw(Gear::GearEngine* engine, Camera* camera, Inputs* inputs,Debug* debug)
{
	int actorID = 0;
	glm::vec3 hitPoint(0.0f);
	glm::vec3 hitNorm(0.f);
	engine->pickActorFromWorld(LevelModelHandler::getInstance()->getModels(), LevelModelHandler::getInstance()->getModelInstanceAgentIDs(), camera, inputs->getMousePos(), actorID, hitPoint, hitNorm);
	debug->drawLine(hitPoint, hitPoint + (hitNorm * 10));

	if (inputs->buttonPressed(GLFW_MOUSE_BUTTON_1))
	{
		LevelActor* newActor = LevelActorFactory::getInstance()->createActor(LevelAssetHandler::getInstance()->getSelectedPrefab());
		if (newActor)
		{
			LevelActorHandler::getInstance()->addActor(newActor);
			LevelActorHandler::getInstance()->setSelected(newActor);

			newActor->setActorType("BRUSH");
			newActor->setActorType(saveAsType);
		
			LevelTransform* transform = newActor->getComponent<LevelTransform>();

			if (transform)
				transform->getTransformRef()->setPos(hitPoint);
		}

	}

}
LevelBrushHandler::LevelBrushHandler()
{

}

LevelBrushHandler * LevelBrushHandler::getInstance()
{
	if (g_instance == nullptr)
		g_instance = new LevelBrushHandler();
	return g_instance;
	//return nullptr;
}

void LevelBrushHandler::deleteInstance()
{
	if (g_instance != nullptr)
		delete g_instance;
}
//
void LevelBrushHandler::resetInstance()
{
	if (g_instance != nullptr)
		delete g_instance;
	g_instance = new LevelBrushHandler();
}

void LevelBrushHandler::updateBrushBar()
{
	//nothing 
}



