#include "ModuleSceneManager.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"


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
		engine->sceneWindow->drawableObjects.erase(engine->sceneWindow->drawableObjects.begin());
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
	GameObject* ret = nullptr;

	if (gameObject != nullptr) {
		ret = new GameObject(*gameObject);

		if (ret->parent == nullptr) {
			sceneRootObjects.push_back(ret);
		}
	}

	return ret;
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
