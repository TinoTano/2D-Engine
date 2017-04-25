#include "ComponentTransform.h"
#include "GameObject.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"


ComponentTransform::ComponentTransform(GameObject* attachedObject)
{
	type = Transform;
	globalPosition = { 0,0,0 };
	localPosition = { 0,0,0 };
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

	if (gameObject->isRoot) {
		gameObject->gameObjectSprite->setPosition(sf::Vector2f(position.x, position.y));
		gameObject->positionZ = position.z;
		globalPosition = localPosition = position;
	}
	else {
		ComponentTransform* transform = (ComponentTransform*)gameObject->parent->GetComponent(Transform);
		globalPosition = transform->globalPosition + localPosition;
		localPosition = position;
		gameObject->gameObjectSprite->setPosition(sf::Vector2f(globalPosition.x, globalPosition.y));
		gameObject->positionZ = globalPosition.z;
	}

	if (gameObject->childs.size() > 0) {
		for (list<GameObject*>::iterator it = gameObject->childs.begin(); it != gameObject->childs.end(); it++) {
			ComponentTransform* transform = (ComponentTransform*)(*it)->GetComponent(Transform);
			transform->SetPosition(transform->localPosition);
		}
	}
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

void ComponentTransform::SetInitialPosition(sf::Vector2f pos)
{
	globalPosition = sf::Vector3f(pos.x, pos.y, 0);
	if (gameObject->isRoot) {
		localPosition = globalPosition;
	}
	else {
		ComponentTransform* transform = (ComponentTransform*)gameObject->parent->GetComponent(Transform);
		localPosition = sf::Vector3f(pos.x - transform->globalPosition.x, pos.y - transform->globalPosition.y, 0);
	}
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
	data.AddVector3Float("Global_Position", globalPosition);
	data.AddVector3Float("Loacal_Position", localPosition);
	data.AddFloat("Rotation", rotation);
	data.AddVector2Float("Scale", scale);
}

void ComponentTransform::Load(Data & data)
{
	//SetGlobalPosition(data.GetVector3Float("Global_Position"));
	//SetLocalPosition(data.GetVector3Float("Local_Position"));
	SetRotation(rotation = data.GetFloat("Rotation"));
	SetScale(data.GetVector2Float("Scale"));
}
