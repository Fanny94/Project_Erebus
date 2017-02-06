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


void TW_CALL setScaleCB(const void *value, void *s /*clientData*/)
{
	LevelBrushHandler::getInstance()->setIsScale(!LevelBrushHandler::getInstance()->getIsScale());
}

void TW_CALL getScaleCB(void *value, void *s /*clientData*/)
{
	*(bool*)value = LevelBrushHandler::getInstance()->getIsScale();
}

void TW_CALL setRotateCB(const void *value, void *s /*clientData*/)
{
	LevelBrushHandler::getInstance()->setIsRotation(!LevelBrushHandler::getInstance()->getIsRotation());
}

void TW_CALL getRotateCB(void *value, void *s /*clientData*/)
{
	*(bool*)value = LevelBrushHandler::getInstance()->getIsRotation();
}


void LevelBrushHandler::setTweakBar(TweakBar * brushBar)
{
	this->actionBar = brushBar;

	TwAddVarRW(actionBar->getBar(), "radius", TW_TYPE_FLOAT, &this->radius, NULL);
	//TwAddSeparator(actionBar->getBar(), "sep2", NULL);
	TwAddVarRW(actionBar->getBar(), "Vacansy", TW_TYPE_FLOAT, &this->VacancyRadius, NULL);
	TwAddVarRW(actionBar->getBar(), "Y_Offset", TW_TYPE_FLOAT, &this->yOffset, NULL);
	TwAddVarCB(actionBar->getBar(), "saveAsType", TW_TYPE_STDSTRING,setSaveTypeCB,getSaveTypeCB,&saveAsType,"");
	TwAddVarCB(actionBar->getBar(), "IsRotate", TW_TYPE_BOOL16, setRotateCB, getRotateCB, (void*)this, "label='Rotate'");
	TwAddVarCB(actionBar->getBar(), "IsScale", TW_TYPE_BOOL16, setScaleCB, getScaleCB, (void*)this, "label='Scale'");
}
void LevelBrushHandler::testDraw(Gear::GearEngine* engine, Camera* camera,const double deltaTime, Inputs* inputs,Debug* debug)
{

	int actorID = 0;
	glm::vec3 hitPoint(0.0f);
	glm::vec3 hitNorm(0.f);
	engine->pickActorFromWorld(LevelModelHandler::getInstance()->getModels(), LevelModelHandler::getInstance()->getModelInstanceAgentIDs(), camera, inputs->getMousePos(), actorID, hitPoint, hitNorm);

	debug->drawLine(hitPoint, hitPoint + (hitNorm * this->radius * 2.5));
	debug->drawSphere(hitPoint, this->radius, glm::vec3(1, 0, 1));

	if (hitNorm.y < 0.65)
	{
		return;
	}

	glm::vec3 newHitPoint;
	
	hitPoint.x = (hitPoint.x += RNG::range((-this->radius),this->radius) );
	hitPoint.z = (hitPoint.z += RNG::range((-this->radius),this->radius) );
	hitPoint.y = hitPoint.y + yOffset;
	
	//jag skulle kunna k�ra en str�le fr�n marken till kameran. och sen k�ra tillbaka f�r att f� ett b�ttre y v�rde.
	//jag borde kolla om det finns mark d�r. Om normalen �r f�r offsetad borde jag inte rita.

	
	timer -= deltaTime;
	
	if (inputs->buttonPressed(GLFW_MOUSE_BUTTON_1) && timer <=0)
	{
		bool drawOver = true;
		if (drawOver)
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
		
			LevelTransform* transform = newActor->getComponent<LevelTransform>();

			if (transform)
			{
				glm::vec3 newNormal = hitNorm;
				newNormal.y = RNG::range(0.0,PIx2);
				transform->getTransformRef()->setPos(hitPoint);
				transform->getChangeTransformRef()->setRotation(newNormal);
			}
			
			timer = 0.2;
			earlierPositions.insert(earlierPositions.begin(),hitPoint);
			earlierPositions.pop_back();

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

void LevelBrushHandler::setIsScale(bool t_f)
{
	isScale = t_f;
}

bool LevelBrushHandler::getIsRotation()
{
	return isRotation;
}

bool LevelBrushHandler::getIsScale()
{
	return isScale;
}



