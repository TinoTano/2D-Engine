#pragma once

#include "Engine.h"

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

public:
	ComponentType type;

private:
	bool active;
	
};

