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
	GameObject* FindGameObjectInverse(string name) const;
	GameObject* CreateGameObject(GameObject* parent = nullptr);
	GameObject* DuplicateGameObject(GameObject* gameObject);
	void RenameDuplicatedGameObject(GameObject* gameObject, bool justIncrease = false);

	void NewScene();
	void LoadScene(string path);
	void SaveScene(string path) const;

	void StartGame();
	void PauseGame();
	void UnPauseGame();
	void StopGame();

public:
	list<GameObject*> selectedGameObjects;
	list<GameObject*> gameObjectsToDestroy;
	list<GameObject*> sceneRootObjects;
	map<string, int> sceneGameObjectsNameCounter;
	vector<GameObject*> gameObjectsList;
	string sceneName = "Untitled Scene";
	map<GameObject*, string> gameObjectsParentsNames;
	int savingIndex = 0;
};

