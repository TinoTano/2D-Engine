#pragma once

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* attachedObject);
	~ComponentTransform();

	void SetPosition(sf::Vector3f position);
	void SetRotation(float degree);
	void SetScale(sf::Vector2f scale);

	void OnEnable();
	void OnDisable();

public:
	sf::Vector3f position;
	float rotation;
	sf::Vector2f scale;
};

