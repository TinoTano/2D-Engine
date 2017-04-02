#pragma once

#include "Engine.h"

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

public:
	ComponentType type;
	GameObject* gameObject = nullptr;
	string name;

private:
	bool active;

	
};

