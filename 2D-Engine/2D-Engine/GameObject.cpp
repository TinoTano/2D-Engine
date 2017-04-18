#include "GameObject.h"
#include "ModuleSceneManager.h"
#include "Engine.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "ModuleSceneWindow.h"
#include "Data.h"

GameObject::GameObject() 
{
	name = "New GameObject";
	tag = "None";
	layer = "None";
	activeInScene = true;
	destroyOnLoadScene = false;
	parent = nullptr;
	AddComponent(Component::Transform);
	AddComponent(Component::SpriteRenderer);
	positionZ = 0;
	isRoot = true;
	onDestroy = false;
}

GameObject::GameObject(const GameObject & gameObject)
{
	Data data;
	gameObject.Save(data);
	AddComponent(Component::Transform);
	AddComponent(Component::SpriteRenderer);
	for (int i = 0; i <= engine->sceneManagerModule->savingIndex; i++) {
		data.EnterSection("GameObject_" + to_string(i));
		if (i == 0) {
			Load(data);
		}
		else {
			GameObject* go = new GameObject();
			go->Load(data);
		}
		data.LeaveSection();
	}
	engine->sceneManagerModule->savingIndex = 0;
	data.ClearData();
}

GameObject::GameObject(string name)
{
	this->name = name;
	activeInScene = true;
	parent = nullptr;
	destroyOnLoadScene = false;
	tag = "None";
	layer = "None";
	AddComponent(Component::Transform);
	AddComponent(Component::SpriteRenderer);
	positionZ = 0;
	isRoot = true;
	onDestroy = false;
}

GameObject::GameObject(GameObject * parent)
{
	name = "New GameObject";
	tag = "None";
	layer = "None";
	activeInScene = true;
	destroyOnLoadScene = false;
	SetParent(parent);
	AddComponent(Component::Transform);
	AddComponent(Component::SpriteRenderer);
	positionZ = 0;
	isRoot = true;
	onDestroy = false;
}


GameObject::~GameObject()
{
	for (list<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); ++it) {
		if (*it != nullptr) {
			RELEASE(*it);
		}
	}

	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it) {
		if (*it != nullptr) {
			RELEASE(*it);
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
	isRoot = false;
	engine->sceneManagerModule->sceneRootObjects.remove(this);
}

void GameObject::SetParentByName(string name)
{
	SetParent(engine->sceneManagerModule->FindGameObject(name));
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
	onDestroy = true;
	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end();) {
		if (*it != nullptr) {
			(*it)->OnDestroy();
			RELEASE(*it);
			it = childs.erase(it);
		}
	}
	for (int i = 0; i < engine->sceneWindow->drawableObjects.size(); i++) {
		if (engine->sceneWindow->drawableObjects[i] == this) {
			engine->sceneWindow->drawableObjects.erase(engine->sceneWindow->drawableObjects.begin() + i);
		}
	}
	if (parent != nullptr && !parent->onDestroy) {
		parent->childs.remove(this);
	}
}

void GameObject::Destroy()
{
	engine->sceneManagerModule->gameObjectsToDestroy.push_back(this);
}

void GameObject::DestroyComponent(Component::ComponentType component)
{
	for (list<Component*>::iterator it = componentsList.begin(); it != componentsList.end();) {
		if ((*it)->type == component) {
			RELEASE(*it);
			it = componentsList.erase(it);
		}
		else {
			it++;
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

void GameObject::Save(Data & data) const
{
	data.CreateSection("GameObject_" + to_string(engine->sceneManagerModule->savingIndex++));
	data.AddString("Name", name);
	data.AddString("Tag", tag);
	data.AddString("Layer", layer);
	data.AddBool("ActiveInScene", activeInScene);
	data.CreateSection("Components");
	data.AddInt("Components_Count", componentsList.size());
	int componentIndex = 0;
	for (list<Component*>::const_iterator it = componentsList.begin(); it != componentsList.end(); it++) {
		data.CreateSection("Component_" + to_string(componentIndex));
		(*it)->Save(data);
		data.CloseSection();
		componentIndex++;
	}
	data.CloseSection();
	if (parent == nullptr) {
		data.AddString("ParentName", "NULL");
	}
	else {
		data.AddString("ParentName", parent->name);
	}
	data.AddFloat("PositionZ", positionZ);
	data.AddBool("IsRoot", isRoot);
	data.CloseSection();
	
	//Save all childs recursively
	for (list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++) {
		(*it)->Save(data);
	}
}

void GameObject::Load(Data & data)
{
	name = data.GetString("Name");
	tag = data.GetString("Tag");
	layer = data.GetString("Layer");
	activeInScene = data.GetBool("ActiveInScene");
	data.EnterSection("Components");
	int componentsCount = data.GetInt("Components_Count");
	for (int i = 0; i < componentsCount; i++) 
	{
		data.EnterSection("Component_" + to_string(i));
		Component* component = GetComponent((Component::ComponentType)data.GetInt("Type"));
		if (component != nullptr) {
			component->Load(data);
		}
		else {
			AddComponent((Component::ComponentType)data.GetInt("Type"));
			GetComponent((Component::ComponentType)data.GetInt("Type"))->Load(data);
		}
		data.LeaveSection();
	}
	data.LeaveSection();
	string parentName = data.GetString("ParentName");
	if (parentName != "NULL") {
		SetParentByName(parentName);
	}
	positionZ = data.GetFloat("PositionZ");
	isRoot = data.GetBool("IsRoot");
	if (isRoot) {
		engine->sceneManagerModule->sceneRootObjects.push_back(this);
	}
}
