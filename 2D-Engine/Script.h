#pragma once

#include <string>
#include "GameObject.h"
#include <SFML\Graphics\Sprite.hpp>
#include "SFML\System\vector2.hpp"
#include "SFML\System\Vector3.hpp"

//#define USING_CPP 0
//#define USING_LUA 1
//#define USING_CS 2
//#define USING_JS 3

using namespace std;

struct ScriptField {
	enum PropertyType {
		Integer, Decimal, Float, Bool, String, GameObject, Animation, Vector2f, Vector2i, Vector3f, Vector3i, Vector4f, Vector4i, Sprite, Audio, None
	};
	string fieldName;
	PropertyType propertyType = None;
};

class Script
{
public:
	Script();
	virtual ~Script();

	void CreateScriptContext(const char* code, GameObject* container);

	virtual bool InitScript(const char* code, GameObject* container) = 0;
	virtual void StartScript() = 0;
	virtual void UpdateScript(float deltaTime) = 0;
	virtual void OnCollisionEnter() = 0;
	virtual void OnCollisionStay() = 0;
	virtual void OnCollisionExit() = 0;
	virtual void OnEnable() = 0;
	virtual void OnDisable() = 0;

	virtual void CallFunction(const char* function) = 0;
	
	virtual void SetIntProperty(const char* propertyName, int value) = 0;
	virtual int GetIntProperty(const char* propertyName) = 0;
	virtual void SetDoubleProperty(const char* propertyName, double value) = 0;
	virtual double GetDoubleProperty(const char* propertyName) = 0;
	virtual void SetFloatProperty(const char* propertyName, float value) = 0;
	virtual float GetFloatProperty(const char* propertyName) = 0;
	virtual void SetBoolProperty(const char* propertyName, bool value) = 0;
	virtual bool GetBoolProperty(const char* propertyName) = 0;
	virtual void SetStringProperty(const char* propertyName, const char* value) = 0;
	virtual string GetStringProperty(const char* propertyName) = 0;
	virtual void SetGameObjectProperty(const char* propertyName, GameObject* value) = 0;
	virtual GameObject* GetGameObjectProperty(const char* propertyName) = 0;
	virtual void SetVec2fProperty(const char* propertyName, sf::Vector2f value) = 0;
	virtual sf::Vector2f GetVec2fProperty(const char* propertyName) = 0;
	virtual void SetVec2iProperty(const char* propertyName, sf::Vector2i value) = 0;
	virtual sf::Vector2i GetVec2iProperty(const char* propertyName) = 0;
	virtual void SetVec3fProperty(const char* propertyName, sf::Vector3f value) = 0;
	virtual sf::Vector3f GetVec3fProperty(const char* propertyName) = 0;
	virtual void SetVec3iProperty(const char* propertyName, sf::Vector3i value) = 0;
	virtual sf::Vector3i GetVec3iProperty(const char* propertyName) = 0;
	virtual void SetVec4iProperty(const char* propertyName, ImVec4 value) = 0;
	virtual ImVec4 GetVec4iProperty(const char* propertyName) = 0;
	virtual void SetSpriteProperty(const char* propertyName, sf::Sprite value) = 0;
	virtual sf::Sprite GetSpriteProperty(const char* propertyName) = 0;

	virtual vector<ScriptField*> GetScriptFields() = 0;
};

