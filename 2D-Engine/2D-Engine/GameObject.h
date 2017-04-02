#pragma once

#include <string>
#include <list>
#include "Component.h"
#include "Engine.h"

using namespace std;

class GameObject
{
public:

	GameObject(const GameObject& gameObject);
	GameObject(string name);
	GameObject(GameObject* parent);
	virtual ~GameObject();

	void AddComponent(Component::ComponentType component);
	Component* GetComponent(Component::ComponentType component);
	void SetActive(bool active);
	bool isActive();
	GameObject* Instantiate(GameObject objectToClone, GameObject* parent);
	void SetParent(GameObject* parent);
	GameObject* GetParent() const;

	void Rename(string name);

	void OnEnable();
	void OnDisable();
	void OnDestroy();

	void Destroy();
	void DestroyComponent(Component component);
	void DestroyAfterTime(float time);
	void DestroyInmediate();
	void DestroyOnLoadScene(bool destroy);

private:
	bool destroyOnLoadScene;

public:
	string name;
	string tag;
	string layer;
	bool activeInScene;
	list<Component*> componentsList;
	list<GameObject*> childs;
	GameObject* parent;
	sf::Sprite* gameObjectSprite;
	float positionZ;
};
