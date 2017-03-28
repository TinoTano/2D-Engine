#pragma once

#include "Module.h"
#include "GameObject.h"

class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager();
	~ModuleSceneManager();

	bool Awake();
	bool PreUpdate();
	bool Update(float deltaTime);
	bool CleanUp();

	GameObject* FindGameObject(string name) const;
	GameObject* CreateGameObject(GameObject* parent = nullptr);
	GameObject* DuplicateGameObject(const GameObject* gameObject);
	void CopyGameObject(const GameObject* gameObject);
	void PasteGameObject(GameObject* gameObject);

	void NewScene();
	void LoadScene();
	void SaveScene();

public:
	list<GameObject*> selectedGameObjects;
	list<GameObject*> gameObjectsToDestroy;
	list<GameObject*> sceneRootObjects;

private:
	
};

