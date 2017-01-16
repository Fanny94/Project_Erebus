#include"LevelPointLightComponent.h"

const char* LevelPointLightComponent::name = "LevelPointLight";

LevelPointLightComponent::LevelPointLightComponent()
{

}

LevelPointLightComponent::~LevelPointLightComponent()
{
	
}

void LevelPointLightComponent::update(float deltaTime) {

	Debugger::getInstance()->drawSphere(this->pos, this->radius);
}

void LevelPointLightComponent::initialize(tinyxml2::XMLElement* element)
{
	this->radius = std::stof(element->FirstChildElement("Radius")->Attribute("r"));
	
	this->color.x = std::stof(element->FirstChildElement("Color")->Attribute("r"));
	this->color.y = std::stof(element->FirstChildElement("Color")->Attribute("g"));
	this->color.z = std::stof(element->FirstChildElement("Color")->Attribute("b"));
	
	this->pos.x = std::stof(element->FirstChildElement("Position")->Attribute("x"));
	this->pos.y = std::stof(element->FirstChildElement("Position")->Attribute("y"));
	this->pos.z = std::stof(element->FirstChildElement("Position")->Attribute("z"));


}

tinyxml2::XMLElement* LevelPointLightComponent::toXml(tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* element = doc->NewElement(LevelPointLightComponent::name);

	//element->LinkEndChild(modelElement);
	//tinyxml2::XMLElement* positionElement = doc->NewElement("Position");
	//positionElement->SetAttribute("x", position.x);
	//positionElement->SetAttribute("y", position.y);
	//positionElement->SetAttribute("z", position.z);

	//tinyxml2::XMLElement* rotationElement = doc->NewElement("Rotation");
	//rotationElement->SetAttribute("x", rotation.x);
	//rotationElement->SetAttribute("y", rotation.y);
	//rotationElement->SetAttribute("z", rotation.z);

	//element->LinkEndChild(positionElement);
	//element->LinkEndChild(rotationElement);

	return element;
}

std::string LevelPointLightComponent::getName()
{
	return LevelPointLightComponent::name;
}

void LevelPointLightComponent::postInitialize()
{
	

}

std::string LevelPointLightComponent::toLua(std::string name)
{
	std::stringstream ss;

	//ss << name << ".model = Assets.LoadModel('Models/" << "GODDAMN" << ".model')" << std::endl;
/*
	if (parent->getComponent<LevelAnimation>())
		ss << "Gear.AddAnimatedInstance(" << name << ".model, " << name << ".transformID, " << name << ".animation)" << std::endl;
	else
		ss << "Gear.AddStaticInstance(" << name << ".model, " << name << ".transformID)" << std::endl;
*/
	return ss.str();
}

float LevelPointLightComponent::getRadius()
{
	return radius;
}
glm::vec3 LevelPointLightComponent::getPos()
{
	return pos;
}
glm::vec3 LevelPointLightComponent::getColor()
{
	return color;
}

void LevelPointLightComponent::setPos(glm::vec3 inPos)
{
	pos = inPos;
}

void LevelPointLightComponent::setColor(glm::vec3 inColor)
{
	color = inColor;
}

void LevelPointLightComponent::setRadius(float inRadius)
{
	radius = inRadius;
}

void LevelPointLightComponent::setTwStruct(TwBar* tempBar )
{
	TwAddVarRW(tempBar, "Pos", LevelUI::TW_TYPE_VECTOR3F(), (void*)&light.pos, NULL);
	TwAddVarRW(tempBar, "Light", TW_TYPE_FLOAT, (void*)&light.radius.x, NULL);
}