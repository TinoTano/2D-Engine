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
	void SetInitialPosition(sf::Vector2f pos);

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

public:
	sf::Vector3f globalPosition;
	sf::Vector3f localPosition;
	float rotation;
	sf::Vector2f scale;
};

