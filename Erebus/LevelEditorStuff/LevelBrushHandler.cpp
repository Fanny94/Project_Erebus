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

void TW_CALL setRotateCB(const void *value, void *s /*clientData*/)
{
	LevelBrushHandler::getInstance()->setIsRotation(!LevelBrushHandler::getInstance()->getIsRotation());
}

void TW_CALL getRotateCB(void *value, void *s /*clientData*/)
{
	*(bool*)value = LevelBrushHandler::getInstance()->getIsRotation();
}

void TW_CALL undoButton(void* args)
{
	LevelBrushHandler::getInstance()->undoLastBrushAction();
}

void LevelBrushHandler::undoLastBrushAction()
{
	if (this->actorsMade.size() > 0)
	{
		for (LevelActor* actor : this->actorsMade.back())
		{
			LevelActorHandler::getInstance()->removeActor(actor);
		}
		
		std::cout << this->actorsMade.size();
		this->actorsMade.pop_back();
	}
}

void LevelBrushHandler::setTweakBar(TweakBar * brushBar)
{
	this->actionBar = brushBar;

	TwAddVarCB(actionBar->getBar(), "Type", TW_TYPE_STDSTRING, setSaveTypeCB, getSaveTypeCB, &saveAsType, NULL);
	TwAddSeparator(actionBar->getBar(), "brushSep1", NULL);

	TwAddVarRW(actionBar->getBar(), "Radius", TW_TYPE_FLOAT, &this->radius, "label=Radius min=0.1");
	TwAddVarRW(actionBar->getBar(), "Vacansy", TW_TYPE_FLOAT, &this->VacancyRadius, "label=Vacansy min=0.1");
	TwAddSeparator(actionBar->getBar(), "brushSep2", NULL);
	
	

	TwAddVarCB(actionBar->getBar(), "isRotate", TW_TYPE_BOOL16, setRotateCB, getRotateCB, (void*)this, "label='Rotate'");
	TwAddSeparator(actionBar->getBar(), "brushSep3", NULL);

	TwAddVarRW(actionBar->getBar(), "Y_Offset", TW_TYPE_FLOAT, &this->yOffset, NULL);

	TwAddVarRW(actionBar->getBar(), "ScaleMin", TW_TYPE_FLOAT, &this->minScale, NULL);
	TwAddVarRW(actionBar->getBar(), "ScaleMax", TW_TYPE_FLOAT, &this->maxScale, NULL);

	TwAddSeparator(actionBar->getBar(), "brushSep4", NULL);
	
	TwAddButton(actionBar->getBar(), "UNDO", undoButton, NULL, "label='Undo'");
}
void LevelBrushHandler::update(Gear::GearEngine* engine, Camera* camera,const double deltaTime, Inputs* inputs,Debug* debug)
{

	if (inputs->buttonReleasedThisFrame(GLFW_MOUSE_BUTTON_1))
	{
		if (actorsMadeThisKeyPress.size()>0)
		{
			actorsMade.push_back(actorsMadeThisKeyPress);
			actorsMadeThisKeyPress.clear();
		}
	}

	//draw debug
	int actorID = 0;
	glm::vec3 hitPoint(0.0f);
	glm::vec3 hitNorm(0.f);
	engine->pickActorFromWorld(LevelModelHandler::getInstance()->getModels(), LevelModelHandler::getInstance()->getModelInstanceAgentIDs(), camera, inputs->getMousePos(), actorID, hitPoint, hitNorm);
	
	debug->drawLine(hitPoint, hitPoint + (hitNorm * this->radius * 2.5));
	debug->drawSphere(hitPoint, this->radius, glm::vec3(1, 0, 1));
	//end of draw

	if (hitNorm.y < 0.65) //dont draw if very steep platform
		return;

	
	glm::vec3 newHitPoint;
	
	hitPoint.x = (hitPoint.x += RNG::range((-this->radius),this->radius) );
	hitPoint.z = (hitPoint.z += RNG::range((-this->radius),this->radius) );
	hitPoint.y = hitPoint.y + yOffset;	
	
	timer -= deltaTime;
	

	if (inputs->buttonPressed(GLFW_MOUSE_BUTTON_1) && timer <=0)
	{
		if (this->preventOverDraw == false)
		{
			for (glm::vec3 position : earlierPositions)
			{
				glm::vec3 result = hitPoint - position;
			
				if ((result.x <= 0 && result.x >(-this->VacancyRadius)) || (result.x >= 0 && result.x < this->VacancyRadius))
				{
					if ((result.z <= 0 && result.z >(-this->VacancyRadius)) || (result.z >= 0 && result.z < this->VacancyRadius))
					{
						return;
					}
				}
			}
		}

		LevelActor* newActor = LevelActorFactory::getInstance()->createActor(LevelAssetHandler::getInstance()->getSelectedPrefab());
		if (newActor)
		{
			LevelActorHandler::getInstance()->addActor(newActor);
			LevelActorHandler::getInstance()->setSelected(newActor);
			newActor->setActorType(saveAsType);
			newActor->setActorDisplayName(LevelActorHandler::getInstance()->tryActorName(newActor->getActorDisplayName()));
		
			LevelTransform* transform = newActor->getComponent<LevelTransform>();

			if (transform)
			{
				glm::vec3 newNormal = hitNorm;
				glm::vec3 scale = transform->getChangeTransformRef()->getScale();
				scale *= RNG::range(this->minScale, this->maxScale);

				if (isRotation)
					newNormal.y = RNG::range(0.0,PIx2);

				transform->getTransformRef()->setPos(hitPoint);
				transform->getChangeTransformRef()->setRotation(newNormal);
				transform->getTransformRef()->setScale(scale);
			}
			
			timer = 0.2;
			earlierPositions.insert(earlierPositions.begin(),hitPoint);
			earlierPositions.pop_back();
		
			this->actorsMadeThisKeyPress.push_back(newActor);
			LevelActorHandler::getInstance()->updateTweakBars();
		}

	}

}
LevelBrushHandler::LevelBrushHandler()
{
	/* Intializes random number generator */
	time_t t;
	srand((unsigned)time(&t));

	for (unsigned int i = 0; i < numSavedPositions; i++) //fill upp the 3 earlier brush positions
	{
		earlierPositions.push_back(glm::vec3(9999, 9999, 9999));
	}
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

void LevelBrushHandler::setIsRotation(bool t_f)
{
	this->isRotation = t_f;
}


bool LevelBrushHandler::getIsRotation()
{
	return isRotation;
}


