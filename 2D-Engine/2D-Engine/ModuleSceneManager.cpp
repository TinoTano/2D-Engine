#include "ModuleSceneManager.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"
#include "Data.h"
#include "ModuleEngineWindow.h"


ModuleSceneManager::ModuleSceneManager()
{
	moduleName = "Scene Manager";
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
	for (list<GameObject*>::iterator it = gameObjectsToDestroy.begin(); it != gameObjectsToDestroy.end();) {
		if (*it != nullptr) {
			(*it)->OnDestroy();
			if ((*it)->isRoot) {
				sceneRootObjects.remove(*it);
			}
			selectedGameObjects.remove(*it);
			RELEASE(*it);
			it = gameObjectsToDestroy.erase(it);
		}
	}
	
	return true;
}

bool ModuleSceneManager::Update(float deltaTime)
{
	return true;
}

bool ModuleSceneManager::CleanUp()
{
	for (list<GameObject*>::iterator it = sceneRootObjects.begin(); it != sceneRootObjects.end(); it++) {
		RELEASE(*it);
	}
	return true;
}

GameObject * ModuleSceneManager::FindGameObject(string name) const
{
	for (int i = 0; i < gameObjectsList.size(); i++) {
		if (gameObjectsList[i]->name == name) {
			return gameObjectsList[i];
		}
	}
	return nullptr;
}

GameObject * ModuleSceneManager::CreateGameObject(GameObject * parent)
{
	GameObject* ret = new GameObject(parent);
	if (parent != nullptr) {
		ret->isRoot = false;
	}
	else {
		sceneRootObjects.push_back(ret);
	}
	map<string, int>::iterator it = sceneGameObjectsNameCounter.find(ret->name);
	if (it != sceneGameObjectsNameCounter.end()) {
		sceneGameObjectsNameCounter[ret->name] += 1;
		ret->Rename(ret->name + "(" + to_string(it->second) + ")");
	}
	else {
		sceneGameObjectsNameCounter[ret->name] = 1;
	}
	gameObjectsList.push_back(ret);
	return ret;
}

GameObject * ModuleSceneManager::DuplicateGameObject(const GameObject * gameObject)
{
	GameObject* ret = nullptr;
	int gameObjectCount = 1;

	if (gameObject != nullptr) {
		ret = new GameObject(*gameObject);
		if (ret->parent == nullptr) {
			ret->isRoot = true;
			sceneRootObjects.push_back(ret);
		}
	}
	bool inParenthesis = false;
	string str;
	for (int i = 0; i < ret->name.size(); i++) {
		if (ret->name[i] == ')') {
			inParenthesis = false;
			if (ret->name[i + 1] == '\0') {
				break;
			}
			else {
				str.clear();
			}
		}
		if (inParenthesis) {
			str.push_back(ret->name[i]);
		}
		if (ret->name[i] == '(') {
			inParenthesis = true;
		}
	}
	if (atoi(str.c_str()) != 0) {
		ret->name.erase(ret->name.end() - (str.length() + 2), ret->name.end());
		gameObjectCount = stoi(str);
	}
	
	map<string, int>::iterator it = sceneGameObjectsNameCounter.find(ret->name);
	if (it != sceneGameObjectsNameCounter.end()) {
		if (sceneGameObjectsNameCounter[ret->name] < gameObjectCount) {
			sceneGameObjectsNameCounter[ret->name] = gameObjectCount + 1;
		}
		else {
			sceneGameObjectsNameCounter[ret->name] += 1;
		}
		ret->Rename(ret->name + "(" + to_string(it->second) + ")");
	}
	else {
		sceneGameObjectsNameCounter[ret->name] = 1;
	}
	gameObjectsList.push_back(ret);
	return ret;
}

void ModuleSceneManager::NewScene()
{
	gameObjectsToDestroy = sceneRootObjects;
	gameObjectsList.clear();
	sceneGameObjectsNameCounter.clear();
	gameObjectsParentsNames.clear();
	sceneName = "Untitled Scene";
}

void ModuleSceneManager::LoadScene(string  path)
{
	Data data;
	if (data.LoadBinary(path)) {
		NewScene();
		sceneName = data.GetString("Scene Name");
		engine->engineWindow->SetSceneName(sceneName);
		int gameObjectsCount = data.GetInt("GameObjects_Count");
		for (int i = 0; i < gameObjectsCount; i++) {
			data.EnterSection("GameObject_" + to_string(i));
			GameObject* gameObject = new GameObject();
			gameObject->Load(data);
			data.LeaveSection();
			gameObjectsList.push_back(gameObject);
			engine->sceneWindow->drawableObjects.push_back(gameObject);
		}
	}
}

void ModuleSceneManager::SaveScene(string  path) const
{
	Data data;
	data.AddString("Scene Name", sceneName);
	data.AddInt("GameObjects_Count", gameObjectsList.size());
	for (list<GameObject*>::const_iterator it = sceneRootObjects.begin(); it != sceneRootObjects.end(); it++) {
		(*it)->Save(data);
	}
	data.SaveAsBinary(path);
}
