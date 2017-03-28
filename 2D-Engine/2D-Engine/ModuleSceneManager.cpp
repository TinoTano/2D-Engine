#include "ModuleSceneManager.h"



ModuleSceneManager::ModuleSceneManager()
{
}


ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Awake()
{
	//load saved scene if exist
	return true;
}

bool ModuleSceneManager::PreUpdate()
{
	for (list<GameObject*>::iterator it = gameObjectsToDestroy.begin(); it != gameObjectsToDestroy.end(); it++) {
		(*it)->OnDestroy();
		delete *it;
		*it = nullptr;
	}
	
	return true;
}

bool ModuleSceneManager::Update(float deltaTime)
{
	return true;
}

bool ModuleSceneManager::CleanUp()
{
	return true;
}

GameObject * ModuleSceneManager::FindGameObject(string name) const
{
	return nullptr;
}

GameObject * ModuleSceneManager::CreateGameObject(GameObject * parent)
{
	GameObject* gameObject = new GameObject(parent);

	if (parent == nullptr) {
		sceneRootObjects.push_back(gameObject);
	}
	
	return gameObject;
}

GameObject * ModuleSceneManager::DuplicateGameObject(const GameObject * gameObject)
{
	return nullptr;
}

void ModuleSceneManager::CopyGameObject(const GameObject * gameObject)
{
}

void ModuleSceneManager::PasteGameObject(GameObject * gameObject)
{
}

void ModuleSceneManager::NewScene()
{
}

void ModuleSceneManager::LoadScene()
{
}

void ModuleSceneManager::SaveScene()
{
}
