#pragma once

#include "Data.h"
#include <string>

using namespace std;

class GameObject;

class Component
{
public:

	enum ComponentType {
		Transform, Camera, RigidBody, SpriteRenderer, BoxCollider, CircleCollider, AudioSource, Animator, Script, Unknown
	};

public:

	Component();
	virtual ~Component();
	virtual void OnEnable();
	virtual void OnDisable();
	void SetActive(bool active);
	bool IsActive() const;
	GameObject* GetGameObject() const;

	virtual void Save(Data& data) const = 0;
	virtual void Load(Data& data) = 0;

public:
	ComponentType type;
	GameObject* gameObject = nullptr;
	string name;

private:
	bool active;

	
};

