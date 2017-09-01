#pragma once

#include "Script.h"
#include <lua.hpp>

struct LuaUtils {
	static void RegisterLibrary(lua_State* luaState, luaL_Reg lib[], const char* libName);
	static bool CallFunction(lua_State* luaState, const char* functionName);
	static bool FunctionExist(lua_State* luaState, const char* functionName);
};

class LuaScript : public Script
{
public:
	LuaScript();
	virtual ~LuaScript();

	virtual bool InitScript(const char* code, GameObject* container) override;
	virtual void StartScript() override;
	virtual void UpdateScript(float deltaTime) override;
	virtual void OnCollisionEnter() override;
	virtual void OnCollisionStay() override;
	virtual void OnCollisionExit() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void CallFunction(const char* function) override;

	virtual void SetIntProperty(const char* propertyName, int value) override;
	virtual int GetIntProperty(const char* propertyName) override;
	virtual void SetDoubleProperty(const char* propertyName, double value) override;
	virtual double GetDoubleProperty(const char* propertyName) override;
	virtual void SetFloatProperty(const char* propertyName, float value) override;
	virtual float GetFloatProperty(const char* propertyName) override;
	virtual void SetBoolProperty(const char* propertyName, bool value) override;
	virtual bool GetBoolProperty(const char* propertyName) override;
	virtual void SetStringProperty(const char* propertyName, const char* value) override;
	virtual string GetStringProperty(const char* propertyName) override;
	virtual void SetGameObjectProperty(const char* propertyName, GameObject* value) override;
	virtual GameObject* GetGameObjectProperty(const char* propertyName) override;
	virtual void SetVec2fProperty(const char* propertyName, sf::Vector2f value) override;
	virtual sf::Vector2f GetVec2fProperty(const char* propertyName) override;
	virtual void SetVec2iProperty(const char* propertyName, sf::Vector2i value) override;
	virtual sf::Vector2i GetVec2iProperty(const char* propertyName) override;
	virtual void SetVec3fProperty(const char* propertyName, sf::Vector3f value) override;
	virtual sf::Vector3f GetVec3fProperty(const char* propertyName) override;
	virtual void SetVec3iProperty(const char* propertyName, sf::Vector3i value) override;
	virtual sf::Vector3i GetVec3iProperty(const char* propertyName) override;
	virtual void SetVec4iProperty(const char* propertyName, ImVec4 value) override;
	virtual ImVec4 GetVec4iProperty(const char* propertyName) override;
	virtual void SetSpriteProperty(const char* propertyName, sf::Sprite value) override;
	virtual sf::Sprite GetSpriteProperty(const char* propertyName) override;

	virtual vector<ScriptField*> GetScriptFields() override;

private:
	void CloseLua();
	static lua_State* LuaNewState();
	static void* LuaAlloc(void *Ud, void *Ptr, size_t OldSize, size_t NewSize);
	static int LuaPanic(lua_State *luaState);
	void GlobalFunctions(lua_State* luaState);
	static int PrintToLog(lua_State* luaState);
	void RegisterAPI(lua_State* luaState);

	//GameObject library
	static int IsGameObjectActive(lua_State* luaState);
	static int SetGameObjectActive(lua_State* luaState);
	static int GetGameObjectPosition(lua_State* luaState);
	static int SetGameObjectPosition(lua_State* luaState);
	static int GetGameObjectGlobalPosition(lua_State* luaState);
	static int GetGameObjectComponent(lua_State* luaState);
	static int AddGameObjectComponent(lua_State* luaState);
	static int GetGameObjectLayer(lua_State* luaState);
	static int SetGameObjectLayer(lua_State* luaState);
	static int GetGameObjectName(lua_State* luaState);
	static int SetGameObjectName(lua_State* luaState);
	static int GetGameObjectTag(lua_State* luaState);
	static int SetGameObjectTag(lua_State* luaState);
	static int GetGameObjectParent(lua_State* luaState);
	static int SetGameObjectParent(lua_State* luaState);
	static int GetGameObjectRoot(lua_State* luaState);
	static int GetGameObjectChild(lua_State* luaState);
	static int GetGameObjectChildByIndex(lua_State* luaState);
	static int GetGameObjectChildsCount(lua_State* luaState);
	static int FindGameObject(lua_State* luaState);
	static int DestroyGameObject(lua_State* luaState);
	static int FlipGameObjectSpriteX(lua_State* luaState);
	static int FlipGameObjectSpriteY(lua_State* luaState);
	static int TintGameObjectSprite(lua_State* luaState);
	static int GetGameObjectSpriteLocalBounds(lua_State* luaState);
	static int GetGameObjectSpriteGlobalBounds(lua_State* luaState);
	static int IsGameObjectAnimationPlaying(lua_State* luaState);
	static int PlayGameObjectAnimation(lua_State* luaState);
	static int StopGameObjectAnimation(lua_State* luaState);
	static int PauseGameObjectAnimation(lua_State* luaState);
	static int IsGameObjectAnimationFinished(lua_State* luaState);
	static int ChangeGameObjectAnimationSpeed(lua_State* luaState);
	static int PlayGameObjectSound(lua_State* luaState);
	static int StopGameObjectSound(lua_State* luaState);
	static int PauseGameObjectSound(lua_State* luaState);
	static int GetGameObjectSoundState(lua_State* luaState);
	static int SetGameObjectSoundVolume(lua_State* luaState);
	static int SetGameObjectSoundPitch(lua_State* luaState);
	static int PlayGameObjectMusic(lua_State* luaState);
	static int StopGameObjectMusic(lua_State* luaState);
	static int PauseGameObjectMusic(lua_State* luaState);
	static int GetGameObjectMusicState(lua_State* luaState);
	static int SetGameObjectMusicVolume(lua_State* luaState);
	static int SetGameObjectMusicPitch(lua_State* luaState);
	static int PlayGameObjectParticleFX(lua_State* luaState);
	static int StopGameObjectParticleFX(lua_State* luaState);
	static int PauseGameObjectParticleFX(lua_State* luaState);
	

	//Globals
	static int Invoke(lua_State* luaState);
	static int Instantiate(lua_State* luaState);
	
	//Math library
	static int Vector4(lua_State* luaState);
	static int Vector3(lua_State* luaState);
	static int Vector2(lua_State* luaState);

	//Input library
	static int IsKeyPressed(lua_State* luaState);
	static int IsKeyReleased(lua_State* luaState);
	static int IsKeyRepeated(lua_State* luaState);
	static int IsMouseButtonPressed(lua_State* luaState);
	static int IsMouseButtonReleased(lua_State* luaState);
	static int IsMouseButtonRepeated(lua_State* luaState);
	
	static void StackDump(lua_State* luaState);

private:
	bool hasStartFunction = false;
	bool hasUpdateFunction = false;
	bool hasOnCollisionEnter = false;
	bool hasOnCollisionStay = false;
	bool hasOnCollisionExit = false;
	bool hasOnEnable = false;
	bool hasOnDisable = false;
	static bool insideFunction;
	lua_State* luaState = NULL;
	vector<ScriptField*> fields;
};