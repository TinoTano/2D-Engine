#pragma once

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	void OnEnable();
	void OnDisable();

public:
	sf::Vector3f position;
	sf::Vector2f rotation;
	sf::Vector2f scale;
};

