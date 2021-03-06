#pragma once

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* attachedObject);
	~ComponentTransform();

	void SetPosition(sf::Vector3f position, bool isGlobal);
	void SetPosition(sf::Vector2f position, bool isGlobal);
	void SetRotation(float degree);
	void SetScale(sf::Vector2f scale);
	void SetInitialPosition(sf::Vector2f pos);

	sf::Vector3f GetLocalPosition() const;
	sf::Vector3f GetGlobalPosition() const;
	float GetRotation() const;
	sf::Vector2f GetScale() const;

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	sf::Vector3f globalPosition;
	sf::Vector3f localPosition;
	float rotation;
	sf::Vector2f scale;
};

