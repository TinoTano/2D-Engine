#include "ComponentTransform.h"
#include "GameObject.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"


ComponentTransform::ComponentTransform(GameObject* attachedObject)
{
	type = Transform;
	position = sf::Vector3f(engine->sceneWindow->window->getSize().x / 2, engine->sceneWindow->window->getSize().y / 2, 0);
	rotation = 0;
	scale = { 1,1 };
	gameObject = attachedObject;
	name = "Transform";
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetPosition(sf::Vector3f position)
{
	gameObject->gameObjectSprite->setPosition(sf::Vector2f(position.x, position.y));
	gameObject->positionZ = position.z;
	this->position = position;
}

void ComponentTransform::SetRotation(float angle)
{
	gameObject->gameObjectSprite->setRotation(angle);
	rotation = angle;
}

void ComponentTransform::SetScale(sf::Vector2f scale)
{
	gameObject->gameObjectSprite->setScale(scale);
	this->scale = scale;
}

void ComponentTransform::OnEnable()
{
}

void ComponentTransform::OnDisable()
{
}

void ComponentTransform::Save(Data & data) const
{
	data.AddInt("Type", type);
	data.AddVector3Float("Position", position);
	data.AddFloat("Rotation", rotation);
	data.AddVector2Float("Scale", scale);
}

void ComponentTransform::Load(Data & data)
{
	SetPosition(data.GetVector3Float("Position"));
	SetRotation(rotation = data.GetFloat("Rotation"));
	SetScale(data.GetVector2Float("Scale"));
}
