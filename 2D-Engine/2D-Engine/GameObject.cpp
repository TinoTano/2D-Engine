#include "GameObject.h"
#include "ModuleSceneManager.h"
#include "Engine.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"


GameObject::GameObject(const GameObject & gameObject)
{
	this->name = gameObject.name;
	this->tag = gameObject.tag;
	this->layer = gameObject.layer;
	this->activeInScene = gameObject.activeInScene;
	this->componentsList = gameObject.componentsList;
	SetParent(gameObject.parent);
	this->destroyOnLoadScene = gameObject.destroyOnLoadScene;
	this->tag = gameObject.tag;
	this->layer = gameObject.layer;
	this->childs = gameObject.childs;
	AddComponent(Component::SpriteRenderer);
	AddComponent(Component::Transform);
}

GameObject::GameObject(string name)
{
	this->name = name;
	tag = "";
	layer = "";
	activeInScene = true;
	parent = nullptr;
	destroyOnLoadScene = false;
	tag = "None";
	layer = "None";
	AddComponent(Component::SpriteRenderer);
	AddComponent(Component::Transform);
}

GameObject::GameObject(GameObject * parent)
{
	name = "New GameObject";
	tag = "None";
	layer = "None";
	activeInScene = true;
	destroyOnLoadScene = false;
	SetParent(parent);
	AddComponent(Component::SpriteRenderer);
	AddComponent(Component::Transform);
}


GameObject::~GameObject()
{
	for (list<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); ++it) {
		if (*it != nullptr) {
			delete *it;
			*it = nullptr;
		}
	}

	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it) {
		if (*it != nullptr) {
			delete *it;
			*it = nullptr;
		}
	}
}

void GameObject::AddComponent(Component::ComponentType componentType)
{

	Component* component = nullptr;

	switch (componentType)
	{
	case Component::Transform:
		componentsList.push_back(component = new ComponentTransform(this));
		break;
	case Component::Camera:
		break;
	case Component::RigidBody:
		break;
	case Component::SpriteRenderer:
		componentsList.push_back(component = new ComponentSpriteRenderer(this));
		break;
	case Component::BoxCollider:
		break;
	case Component::CircleCollider:
		break;
	case Component::AudioSource:
		break;
	case Component::Animator:
		break;
	case Component::Script:
		break;
	default:
		break;
	}
}

void GameObject::SetActive(bool active)
{
	activeInScene = active;
}

bool GameObject::isActive()
{
	return activeInScene;
}

Component* GameObject::GetComponent(Component::ComponentType component)
{
	for (list<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); it++) {
		if ((*it)->type == component) {
			return (*it);
		}
	}
	return nullptr;
}

GameObject* GameObject::Instantiate(GameObject objectToClone, GameObject* parent)
{
	GameObject* ret = nullptr;
	ret = new GameObject(objectToClone);
	ret->SetParent(parent);
	return ret;
}

void GameObject::SetParent(GameObject* parent)
{
	if (this->parent == parent) {
		return;
	}

	if (this->parent != nullptr) {
		this->parent->childs.remove(this);
	}

	this->parent = parent;
	this->parent->childs.push_back(this);
	
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::Rename(string name)
{
	this->name = name;
}

void GameObject::OnEnable()
{
}

void GameObject::OnDisable()
{
}

void GameObject::OnDestroy()
{
	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++) {
		if (*it != nullptr) {
			(*it)->OnDestroy();
			childs.erase(it);
			delete *it;
			*it = nullptr;
		}
	}
}

void GameObject::Destroy()
{
	engine->sceneManagerModule->gameObjectsToDestroy.push_back(this);
}

void GameObject::DestroyComponent(Component component)
{
	for (list<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); it++) {
		if ((*it) == &component) {
			if (*it != nullptr) {
				componentsList.erase(it);
				delete *it;
				*it = nullptr;
			}
		}
	}
}

void GameObject::DestroyAfterTime(float time)
{

}

void GameObject::DestroyInmediate()
{
	OnDestroy();
}

void GameObject::DestroyOnLoadScene(bool destroy)
{
	destroyOnLoadScene = destroy;
}
