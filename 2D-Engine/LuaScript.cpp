#include "LuaScript.h"
#include "Globals.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "ComponentScript.h"
#include "ComponentAnimation.h"
#include "Component.h"
#include "ModuleSceneManager.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ResourceAnimation.h"
#include "ComponentAudio.h"
#include "ComponentParticle.h"

bool LuaScript::insideFunction = false;

LuaScript::LuaScript()
{
	
}


LuaScript::~LuaScript()
{
	CloseLua();
}

bool LuaScript::InitScript(const char* code, GameObject * container)
{
	CloseLua();

	insideFunction = false;
	bool ret = false;
	luaState = LuaNewState();
	luaL_openlibs(luaState);
	GlobalFunctions(luaState);
	RegisterAPI(luaState);

	if (luaL_loadstring(luaState, code) == 0) {
		lua_newtable(luaState);
		lua_pushstring(luaState, "Type");
		lua_pushstring(luaState, "GameObject");
		lua_settable(luaState, -3);
		lua_pushstring(luaState, "Data");
		lua_pushlightuserdata(luaState, container);
		lua_settable(luaState, -3);
		lua_setglobal(luaState, "this");
		ret = LuaUtils::CallFunction(luaState, NULL);
		if (ret) {
			hasStartFunction = LuaUtils::FunctionExist(luaState, "Start");
			hasUpdateFunction = LuaUtils::FunctionExist(luaState, "Update");
			hasOnCollisionEnter = LuaUtils::FunctionExist(luaState, "OnCollisionEnter");
			hasOnCollisionStay = LuaUtils::FunctionExist(luaState, "OnCollisionStay");
			hasOnCollisionExit = LuaUtils::FunctionExist(luaState, "OnCollisionExit");
			hasOnEnable = LuaUtils::FunctionExist(luaState, "OnEnable");
			hasOnDisable = LuaUtils::FunctionExist(luaState, "OnDisable");
			insideFunction = true;
		}
	}
	else {
		LOG_WARNING("Cannot load lua script of '%s': %s", container->name.c_str(), lua_tostring(luaState, -1));
	}
	return ret;
}

void LuaScript::StartScript()
{
	if (luaState && hasStartFunction) {
		LuaUtils::CallFunction(luaState, "Start");
	}
}

void LuaScript::UpdateScript(float deltaTime)
{
	if (luaState && hasUpdateFunction) {
		LuaUtils::CallFunction(luaState, "Update");
	}
}

void LuaScript::OnCollisionEnter()
{
	if (luaState && hasOnCollisionEnter) {
		LuaUtils::CallFunction(luaState, "OnCollisionEnter");
	}
}

void LuaScript::OnCollisionStay()
{
	if (luaState && hasOnCollisionStay) {
		LuaUtils::CallFunction(luaState, "OnCollisionStay");
	}
}

void LuaScript::OnCollisionExit()
{
	if (luaState && hasOnCollisionExit) {
		LuaUtils::CallFunction(luaState, "OnCollisionExit");
	}
}

void LuaScript::OnEnable()
{
	if (luaState && hasOnEnable) {
		LuaUtils::CallFunction(luaState, "OnEnable");
	}
}

void LuaScript::OnDisable()
{
	if (luaState && hasOnDisable) {
		LuaUtils::CallFunction(luaState, "OnDisable");
	}
}

void LuaScript::CallFunction(const char * function)
{
	if (LuaUtils::FunctionExist(luaState, function)) {
		LuaUtils::CallFunction(luaState, function);
	}
	else {
		LOG_WARNING("Trying to call non-existent function %s", function);
	}
}

void LuaScript::SetIntProperty(const char* propertyName, int value)
{
	SetFloatProperty(propertyName, (float)value);
}

int LuaScript::GetIntProperty(const char* propertyName)
{
	return (int)GetFloatProperty(propertyName);
}

void LuaScript::SetDoubleProperty(const char* propertyName, double value)
{
	SetFloatProperty(propertyName, (float)value);
}

double LuaScript::GetDoubleProperty(const char* propertyName)
{
	return (double)GetFloatProperty(propertyName);
}

void LuaScript::SetFloatProperty(const char* propertyName, float value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);

	if (lua_isnumber(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_pushnumber(luaState, value);
		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is not a number.", propertyName);
	}
}

float LuaScript::GetFloatProperty(const char* propertyName)
{
	float ret = 0;
	lua_getglobal(luaState, propertyName);
	int ewf = lua_gettop(luaState);
	if (lua_isnumber(luaState, lua_gettop(luaState)))
	{
		ret = (float)lua_tonumber(luaState, -1); // -1 = top of stack
	}
	else
	{
		LOG_WARNING("Global value '%s' is not a number.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetBoolProperty(const char* propertyName, bool value)
{
	bool exists = false;

	lua_getglobal(luaState, propertyName);
	if (lua_isboolean(luaState, lua_gettop(luaState)))
	{
		exists = true;
	}

	// Pop after getglobal, since no other lua function is popping the stack
	lua_pop(luaState, 1);

	if (exists)
	{
		lua_pushboolean(luaState, (int)value);
		lua_setglobal(luaState, propertyName); // setglobal pops the stack for us
	}
	else
	{
		LOG_WARNING("Global value '%s' is not a boolean.", propertyName);
	}
}

bool LuaScript::GetBoolProperty(const char* propertyName)
{
	bool ret = false;
	lua_getglobal(luaState, propertyName);

	if (lua_isboolean(luaState, lua_gettop(luaState)))
	{
		ret = lua_toboolean(luaState, -1); // -1 = top of stack
	}
	else
	{
		LOG_WARNING("Global value '%s' is not a boolean.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetStringProperty(const char* propertyName, const char* value)
{
	bool exists = false;

	lua_getglobal(luaState, propertyName);
	if (lua_isstring(luaState, lua_gettop(luaState)))
	{
		exists = true;
	}

	lua_pop(luaState, 1);

	if (exists)
	{
		lua_pushstring(luaState, value);
		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is not a String.", propertyName);
	}
}

string LuaScript::GetStringProperty(const char* propertyName)
{
	string ret = "";
	lua_getglobal(luaState, propertyName);

	if (lua_isstring(luaState, lua_gettop(luaState)))
	{
		ret = (string)lua_tostring(luaState, -1);
	}
	else
	{
		LOG_WARNING("Global value '%s' is not a String.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetGameObjectProperty(const char* propertyName, GameObject * value)
{
	bool exists = false;

	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState)))
	{
		lua_pushstring(luaState, "Type");
		lua_gettable(luaState, -2);
		if (lua_isstring(luaState, -1)) {
			string str = lua_tostring(luaState, -1);
			if (str == "GameObject")
			{
				exists = true;
			}
		}
	}

	lua_pop(luaState, 1);

	if (exists)
	{
		lua_newtable(luaState);
		lua_pushstring(luaState, "Type");
		lua_pushstring(luaState, "GameObject");
		lua_settable(luaState, -3);
		lua_pushstring(luaState, "Data");
		lua_pushlightuserdata(luaState, value);
		lua_settable(luaState, -3);
		lua_setglobal(luaState, propertyName); // setglobal pops the stack for us
	}
	else
	{
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}
}

GameObject * LuaScript::GetGameObjectProperty(const char* propertyName)
{
	GameObject* ret = nullptr;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState)))
	{
		lua_pushstring(luaState, "Type");
		lua_gettable(luaState, -2);
		if (lua_isstring(luaState, -1)) {
			string str = lua_tostring(luaState, -1);
			if (str == "GameObject")
			{
				lua_pop(luaState, 1);
				lua_pushstring(luaState, "Data");
				lua_gettable(luaState, -2);
				ret = (GameObject*)lua_touserdata(luaState, -1);
			}
		}
	}
	else
	{
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}

	lua_pop(luaState, 1);
	return ret;
}

void LuaScript::SetVec2fProperty(const char* propertyName, sf::Vector2f value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}
}

sf::Vector2f LuaScript::GetVec2fProperty(const char* propertyName)
{
	sf::Vector2f v2f;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v2f.x = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v2f.y = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v2f;
}

void LuaScript::SetVec2iProperty(const char* propertyName, sf::Vector2i value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}
}

sf::Vector2i LuaScript::GetVec2iProperty(const char* propertyName)
{
	sf::Vector2i v2i;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v2i.x = lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v2i.y = lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v2i;
}

void LuaScript::SetVec3fProperty(const char* propertyName, sf::Vector3f value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_pushnumber(luaState, value.z);
		lua_rawseti(luaState, -2, 3);

		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}
}

sf::Vector3f LuaScript::GetVec3fProperty(const char* propertyName)
{
	sf::Vector3f v3f;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3f.x = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3f.y = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 3);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3f.z = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'z' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v3f;
}

void LuaScript::SetVec3iProperty(const char* propertyName, sf::Vector3i value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_pushnumber(luaState, value.z);
		lua_rawseti(luaState, -2, 3);

		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}
	
}

sf::Vector3i LuaScript::GetVec3iProperty(const char* propertyName)
{
	sf::Vector3i v3;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3.x = lua_tointeger(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3.y = lua_tointeger(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 3);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v3.z = lua_tointeger(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'z' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v3;
}

void LuaScript::SetVec4iProperty(const char * propertyName, ImVec4 value)
{
	bool exist = false;
	lua_getglobal(luaState, propertyName);
	if (lua_istable(luaState, lua_gettop(luaState))) {
		exist = true;
	}
	lua_pop(luaState, 1);

	if (exist) {
		lua_newtable(luaState);

		lua_pushnumber(luaState, value.x);
		lua_rawseti(luaState, -2, 1);

		lua_pushnumber(luaState, value.y);
		lua_rawseti(luaState, -2, 2);

		lua_pushnumber(luaState, value.z);
		lua_rawseti(luaState, -2, 3);

		lua_pushnumber(luaState, value.w);
		lua_rawseti(luaState, -2, 3);

		lua_setglobal(luaState, propertyName);
	}
	else {
		LOG_WARNING("Global value '%s' is nil.", propertyName);
	}
}

ImVec4 LuaScript::GetVec4iProperty(const char * propertyName)
{
	ImVec4 v4;
	lua_getglobal(luaState, propertyName);

	if (lua_istable(luaState, lua_gettop(luaState))) {
		lua_pushinteger(luaState, 1);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.x = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'x' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 2);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.y = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'y' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 3);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.z = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'z' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);

		lua_pushinteger(luaState, 4);
		lua_gettable(luaState, -2);
		if (lua_isnumber(luaState, -1)) {
			v4.w = (float)lua_tonumber(luaState, -1);
		}
		else {
			LOG_WARNING("Value 'w' of %s is not a number", propertyName);
		}
		lua_pop(luaState, 1);
	}

	lua_pop(luaState, 1);
	return v4;
}

void LuaScript::SetSpriteProperty(const char* propertyName, sf::Sprite value)
{
}

sf::Sprite LuaScript::GetSpriteProperty(const char* propertyName)
{
	return sf::Sprite();
}

vector<ScriptField*> LuaScript::GetScriptFields()
{
	fields.clear();

	lua_pushglobaltable(luaState);
	lua_pushnil(luaState);

	while (lua_next(luaState, -2) != 0) {
		int keyType = lua_type(luaState, -2);
		if (keyType != LUA_TSTRING) {
			lua_pop(luaState, 1);
			continue;
		}

		int valueType = lua_type(luaState, -1);
		string keyName(lua_tostring(luaState, -2));
		if (keyName.length() == 0 || keyName[0] == '_' || keyName == "this") {
			lua_pop(luaState, 1);
			continue;
		}

		ScriptField* propertyField = new ScriptField();
		propertyField->fieldName = keyName;

		switch (valueType) {
		case LUA_TNUMBER:
		{
			double n = lua_tonumber(luaState, -1);
			if (n == (int)n) {
				propertyField->propertyType = propertyField->Integer;
			}
			else {
				propertyField->propertyType = propertyField->Float;
			}
		}
			break;
		case LUA_TSTRING:
			propertyField->propertyType = propertyField->String;
			break;
		case LUA_TBOOLEAN:
			propertyField->propertyType = propertyField->Bool;
			break;
		case LUA_TLIGHTUSERDATA:
			propertyField->propertyType = propertyField->GameObject;
			break;
		case LUA_TTABLE:
		{
			if (lua_rawlen(luaState, -1) == 3) {
				propertyField->propertyType = propertyField->Vector3i;
			}
			else if (lua_rawlen(luaState, -1) == 2) {
				propertyField->propertyType = propertyField->Vector2i;
			}
			else if (lua_rawlen(luaState, -1) == 4) {
				propertyField->propertyType = propertyField->Vector4i;
			}
		}
			break;
		default:
			propertyField->propertyType = propertyField->None;
			break;
		}
		if(propertyField->propertyType != propertyField->None) {
			fields.push_back(propertyField);
		}

		lua_pop(luaState, 1);
	}

	return fields;
}

void LuaScript::CloseLua()
{
	if (luaState)
	{
		lua_close(luaState);
		luaState = NULL;
	}
}

lua_State * LuaScript::LuaNewState()
{
	lua_State* luaState = lua_newstate(LuaAlloc, NULL);
	if (luaState)
	{
		lua_atpanic(luaState, &LuaPanic);
	}
	return luaState;
}

void * LuaScript::LuaAlloc(void * Ud, void * Ptr, size_t OldSize, size_t NewSize)
{
	if (NewSize != 0)
	{
		return realloc(Ptr, NewSize);
	}
	else
	{
		free(Ptr);
		return NULL;
	}
}

int LuaScript::LuaPanic(lua_State * lua_State)
{
	LOG_ERROR("PANIC: unprotected error in call to Lua API: %s", lua_tostring(lua_State, -1));
	return 0;
}

void LuaScript::GlobalFunctions(lua_State * luaState)
{
	static const luaL_Reg _GFunctions[] =
	{
		{ "print", PrintToLog },
		{ "Invoke", Invoke },
		{ "Instantiate", Instantiate },
		{ NULL, NULL }
	};

	lua_getglobal(luaState, "_G");
	luaL_setfuncs(luaState, _GFunctions, 0);
	lua_pop(luaState, 1);
}

int LuaScript::PrintToLog(lua_State * luaState)
{
	int ArgCount = lua_gettop(luaState);
	string message;

	for (int ArgIndex = 1; ArgIndex <= ArgCount; ++ArgIndex)
	{
		if (lua_isstring(luaState, ArgIndex))
		{
			message += lua_tostring(luaState, ArgIndex);
		}
		else if (lua_isboolean(luaState, ArgIndex)) {
			bool boolArg = lua_toboolean(luaState, ArgIndex);
			if (boolArg) {
				message += "true";
			}
			else {
				message += "false";
			}
		}
		else if (lua_isnumber(luaState, ArgIndex)) {
			message += to_string(lua_tonumber(luaState, ArgIndex));
		}
		else if (lua_istable(luaState, ArgIndex)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject") {
					message += "GameObject!";
				}
			}
		}
		else {
			LOG_ERROR("Don't recognise print function argument type");
			return 0;
		}
	}
	LOG("%s", message.c_str());
	return 0;
}

void LuaScript::RegisterAPI(lua_State* luaState)
{
	luaL_Reg GameObject[] = 
	{
		{ "SetActive", SetGameObjectActive },
		{ "IsActive", IsGameObjectActive },
		{ "GetPosition", GetGameObjectPosition },
		{ "SetPosition", SetGameObjectPosition },
		{ "GetWorldPosition", GetGameObjectGlobalPosition },
		/*{ "GetComponent", GetGameObjectComponent },
		{ "AddComponent", AddGameObjectComponent },*/
		{ "GetLayer", GetGameObjectLayer },
		{ "SetLayer", SetGameObjectLayer },
		{ "GetName", GetGameObjectName },
		{ "SetName", SetGameObjectName },
		{ "GetTag", GetGameObjectTag },
		{ "SetTag", SetGameObjectTag },
		{ "GetParent", GetGameObjectParent },
		{ "SetParent", SetGameObjectParent },
		{ "GetRoot", GetGameObjectRoot },
		{ "GetChild", GetGameObjectChild},
		{ "GetChildByIndex", GetGameObjectChildByIndex },
		{ "ChildsCount", GetGameObjectChildsCount },
		{ "FindGameObject", FindGameObject },
		{ "Destroy", DestroyGameObject },
		{ "FlipSpriteX", FlipGameObjectSpriteX },
		{ "FlipSpriteY", FlipGameObjectSpriteY },
		{ "TintSprite", TintGameObjectSprite },
		{ "GetSpriteLocalBounds", GetGameObjectSpriteLocalBounds },
		{ "GetSpriteGlobalBounds", GetGameObjectSpriteGlobalBounds },
		{ "IsAnimationPlaying", IsGameObjectAnimationPlaying },
		{ "PlayAnimation", PlayGameObjectAnimation },
		{ "StopAnimation", StopGameObjectAnimation },
		{ "PauseAnimation", PauseGameObjectAnimation },
		{ "IsAnimationFinished", IsGameObjectAnimationFinished },
		{ "PlaySound", PlayGameObjectSound },
		{ "StopSound", StopGameObjectSound },
		{ "PauseSound", PauseGameObjectSound },
		{ "GetSoundState", GetGameObjectSoundState },
		{ "SetSoundVolume", SetGameObjectSoundVolume },
		{ "SetSoundPitch", SetGameObjectSoundPitch },
		{ "PlayMusic", PlayGameObjectMusic },
		{ "StopMusic", StopGameObjectMusic },
		{ "PauseMusic", PauseGameObjectMusic },
		{ "GetMusicState", GetGameObjectMusicState },
		{ "SetMusicVolume", SetGameObjectMusicVolume },
		{ "SetMusicPitch", SetGameObjectMusicPitch },
		{ NULL, NULL }
	};
	LuaUtils::RegisterLibrary(luaState, GameObject, "gameObject");

	luaL_Reg Math[] = 
	{
		{"Vector2", Vector2 },
		{"Vector3", Vector3 },
		{ "Vector4", Vector4 },
		{NULL, NULL}
	};
	LuaUtils::RegisterLibrary(luaState, Math, "math");

	luaL_Reg Input[] =
	{
		{ "IsKeyPressed", IsKeyPressed },
		{ "IsKeyReleased", IsKeyReleased },
		{ "IsKeyRepeated", IsKeyRepeated },
		{ "IsMouseButtonPressed", IsMouseButtonPressed },
		{ "IsMouseButtonReleased", IsMouseButtonReleased },
		{ "IsMouseButtonRepeated", IsMouseButtonRepeated },
		{ NULL, NULL }
	};
	LuaUtils::RegisterLibrary(luaState, Input, "input");
}

/////////////// GameObject //////////////////

int LuaScript::IsGameObjectActive(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsActive(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("IsActive() GameObject argument is nil!");
						return 0;
					}
					lua_pushboolean(luaState, (int)go->isActive());
				}
				else {
					LOG_WARNING("IsActive() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("IsActive() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectActive(lua_State * luaState)
{
	if (insideFunction) {
		int arguments = lua_gettop(luaState);
		if (arguments != 2) {
			LOG_WARNING("SetActive(GameObject, bool) takes 2 arguments!");
		}
		else {
			if (lua_istable(luaState, 1)) {
				lua_pushstring(luaState, "Type");
				lua_gettable(luaState, -3);
				if (lua_isstring(luaState, -1)) {
					string str = lua_tostring(luaState, -1);
					if (str == "GameObject")
					{
						lua_pop(luaState, 1);
						lua_pushstring(luaState, "Data");
						lua_gettable(luaState, -3);
						GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
						if (go == nullptr) {
							LOG_WARNING("SetActive() GameObject argument is nil!");
						}
						else {
							if (lua_isboolean(luaState, 2)) {
								bool active = lua_toboolean(luaState, 2);
								go->SetActive(active);
							}
							else {
								LOG_WARNING("SetActive() bool argument is not a boolean!");
							}
						}
					}
					else {
						LOG_WARNING("SetActive() GameObject argument is not a GameObject!");
					}
				}
			}
			else {
				LOG_WARNING("SetActive() GameObject argument is not a GameObject!");
			}
		}
	}
	else {
		LOG_WARNING("SetActive() cannot be called outside Main functions!");
	}
	return 0;
}

int LuaScript::GetGameObjectPosition(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetPosition(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetPosition() GameObject argument is nil!");
						return 0;
					}
					ComponentTransform* transform = (ComponentTransform*)go->GetComponent(ComponentTransform::Transform);
					if (transform == nullptr) {
						LOG_ERROR("GetPosition() failed to get component transform");
						return 0;
					}
					lua_newtable(luaState);
					lua_pushnumber(luaState, transform->GetLocalPosition().x);
					lua_rawseti(luaState, -2, 1);
					lua_pushnumber(luaState, transform->GetLocalPosition().y);
					lua_rawseti(luaState, -2, 2);
					lua_pushnumber(luaState, transform->GetLocalPosition().z);
					lua_rawseti(luaState, -2, 3);
				}
				else {
					LOG_WARNING("GetPosition() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetPosition() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectPosition(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("SetPosition(GameObject, Vector3) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetPosition() GameObject argument is nil!");
					}
					else {
						lua_pop(luaState, 1);
						if (lua_istable(luaState, 2)) {
							ComponentTransform* transform = (ComponentTransform*)go->GetComponent(ComponentTransform::Transform);
							if (transform == nullptr) {
								LOG_ERROR("SetPosition() failed to get component transform");
							}
							else {
								if (lua_rawlen(luaState, 2) == 3) {
									sf::Vector3f v3;
									lua_pushinteger(luaState, 1);
									lua_gettable(luaState, -2);
									if (lua_isnumber(luaState, -1)) {
										v3.x = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("SetPosition() value 'x' is not a number");
									}
									lua_pop(luaState, 1);

									lua_pushinteger(luaState, 2);
									lua_gettable(luaState, -2);
									if (lua_isnumber(luaState, -1)) {
										v3.y = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("SetPosition() value 'y' is not a number");
									}
									lua_pop(luaState, 1);

									lua_pushinteger(luaState, 3);
									lua_gettable(luaState, -2);
									if (lua_isnumber(luaState, -1)) {
										v3.z = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("SetPosition() value 'z' is not a number");
									}
									lua_pop(luaState, 1);
									transform->SetPosition(v3, false);
								}
								else if (lua_rawlen(luaState, 2) == 2) {
									sf::Vector2f v2;
									lua_pushinteger(luaState, 1);
									lua_gettable(luaState, -2);
									if (lua_isnumber(luaState, -1)) {
										v2.x = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("SetPosition() value 'x' is not a number");
									}
									lua_pop(luaState, 1);

									lua_pushinteger(luaState, 2);
									lua_gettable(luaState, -2);
									if (lua_isnumber(luaState, -1)) {
										v2.y = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("SetPosition() value 'y' is not a number");
									}
									lua_pop(luaState, 1);
									transform->SetPosition(v2, false);
								}
								else {
									LOG_WARNING("SetPosition() Vector3 argument is not a Vector3!");
									return 0;
								}
							}
						}
						else {
							LOG_WARNING("SetPosition() Vector3 argument is not a Vector3!");
							return 0;
						}
						lua_pop(luaState, 1);
					}
				}
				else {
					LOG_WARNING("SetPosition() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetPosition() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectGlobalPosition(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetWorldPosition(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetWorldPosition() GameObject argument is nil");
						return 0;
					}
					ComponentTransform* transform = (ComponentTransform*)go->GetComponent(ComponentTransform::Transform);
					if (transform == nullptr) {
						LOG_ERROR("GetWorldPosition() failed to get component transform");
						return 0;
					}
					lua_newtable(luaState);
					lua_pushnumber(luaState, transform->GetGlobalPosition().x);
					lua_rawseti(luaState, -2, 1);
					lua_pushnumber(luaState, transform->GetGlobalPosition().y);
					lua_rawseti(luaState, -2, 2);
					lua_pushnumber(luaState, transform->GetGlobalPosition().z);
					lua_rawseti(luaState, -2, 3);
				}
				else {
					LOG_WARNING("GetWorldPosition() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetWorldPosition() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

//int LuaScript::SetGameObjectGlobalPosition(lua_State * luaState)
//{
//	int arguments = lua_gettop(luaState);
//	if (arguments != 2) {
//		LOG_WARNING("SetGlobalPosition(GameObject, Vector3) takes 2 arguments!");
//	}
//	else {
//		if (lua_istable(luaState, 1)) {
//			lua_pushstring(luaState, "Type");
//			lua_gettable(luaState, -3);
//			if (lua_isstring(luaState, -1)) {
//				string str = lua_tostring(luaState, -1);
//				if (str == "GameObject")
//				{
//					lua_pop(luaState, 1);
//					lua_pushstring(luaState, "Data");
//					lua_gettable(luaState, -3);
//					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
//					if (go == nullptr) {
//						LOG_WARNING("SetGlobalPosition() GameObject argument is nil!");
//					}
//					else {
//						lua_pop(luaState, 1);
//						if (lua_istable(luaState, 2)) {
//							ComponentTransform* transform = (ComponentTransform*)go->GetComponent(ComponentTransform::Transform);
//							if (transform == nullptr) {
//								LOG_WARNING("SetGlobalPosition() failed to get component transform");
//							}
//							else {
//								if (lua_rawlen(luaState, 2) == 3) {
//									sf::Vector3f v3;
//									lua_pushinteger(luaState, 1);
//									lua_gettable(luaState, -2);
//									if (lua_isnumber(luaState, -1)) {
//										v3.x = lua_tonumber(luaState, -1);
//									}
//									else {
//										LOG_WARNING("SetGlobalPosition() value 'x' is not a number");
//									}
//									lua_pop(luaState, 1);
//
//									lua_pushinteger(luaState, 2);
//									lua_gettable(luaState, -2);
//									if (lua_isnumber(luaState, -1)) {
//										v3.y = lua_tonumber(luaState, -1);
//									}
//									else {
//										LOG_WARNING("SetGlobalPosition() value 'y' is not a number");
//									}
//									lua_pop(luaState, 1);
//
//									lua_pushinteger(luaState, 3);
//									lua_gettable(luaState, -2);
//									if (lua_isnumber(luaState, -1)) {
//										v3.z = lua_tonumber(luaState, -1);
//									}
//									else {
//										LOG_WARNING("SetGlobalPosition() value 'z' is not a number");
//									}
//									lua_pop(luaState, 1);
//									transform->SetPosition(v3, true);
//								}
//								else if (lua_rawlen(luaState, 2) == 2) {
//									sf::Vector2f v2;
//									lua_pushinteger(luaState, 1);
//									lua_gettable(luaState, -2);
//									if (lua_isnumber(luaState, -1)) {
//										v2.x = lua_tonumber(luaState, -1);
//									}
//									else {
//										LOG_WARNING("SetGlobalPosition() value 'x' is not a number");
//									}
//									lua_pop(luaState, 1);
//
//									lua_pushinteger(luaState, 2);
//									lua_gettable(luaState, -2);
//									if (lua_isnumber(luaState, -1)) {
//										v2.y = lua_tonumber(luaState, -1);
//									}
//									else {
//										LOG_WARNING("SetGlobalPosition() value 'y' is not a number");
//									}
//									lua_pop(luaState, 1);
//									transform->SetPosition(v2, true);
//								}
//								else {
//									LOG_WARNING("SetGlobalPosition() Vector3 argument is not a Vector3!");
//									return 0;
//								}
//							}
//						}
//						else {
//							LOG_WARNING("SetGlobalPosition() Vector3 argument is not a Vector3!");
//							return 0;
//						}
//						lua_pop(luaState, 1);
//					}
//				}
//				else {
//					LOG_WARNING("SetGlobalPosition() GameObject argument is not a GameObject!");
//				}
//			}
//		}
//		else {
//			LOG_WARNING("SetGlobalPosition() GameObject argument is not a GameObject!");
//		}
//	}
//	return 0;
//}

//int LuaScript::GetGameObjectComponent(lua_State * luaState)
//{
//	int arguments = lua_gettop(luaState);
//	if (arguments != 2) {
//		LOG_WARNING("GetComponent(GameObject, string) takes 2 arguments!");
//		return 0;
//	}
//	if (lua_istable(luaState, 1)) {
//		lua_pushstring(luaState, "Type");
//		lua_gettable(luaState, -3);
//		if (lua_isstring(luaState, -1)) {
//			string str = lua_tostring(luaState, -1);
//			if (str == "GameObject")
//			{
//				lua_pop(luaState, 1);
//				lua_pushstring(luaState, "Data");
//				lua_gettable(luaState, -3);
//				GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
//				if (go == nullptr) {
//					LOG_WARNING("GetComponent() GameObject argument is nil!");
//					return 0;
//				}
//				else {
//					if (lua_isstring(luaState, 2)) {
//						string componentName = lua_tostring(luaState, 2);
//						if (componentName == "SpriteRenderer") {
//							ComponentSpriteRenderer* renderer = (ComponentSpriteRenderer*)go->GetComponent(Component::SpriteRenderer);
//							if (renderer != nullptr) {
//								lua_pushlightuserdata(luaState, renderer);
//							}
//							else {
//								LOG_WARNING("SpriteRenderer component doesn't exists!");
//								return 0;
//							}
//						}
//						else {
//							LOG_WARNING("GetComponent() string argument is not a valid component type");
//							return 0;
//						}
//					}
//					else {
//						LOG_WARNING("GetComponent() string argument is not a string!");
//						return 0;
//					}
//				}
//			}
//			else {
//				LOG_WARNING("GetComponent() GameObject argument is not a GameObject!");
//				return 0;
//			}
//		}
//	}
//	else {
//		LOG_WARNING("GetComponent() GameObject argument is not a GameObject!");
//		return 0;
//	}
//	return 1;
//}

int LuaScript::AddGameObjectComponent(lua_State * luaState)
{
	//int arguments = lua_gettop(luaState);
	//if (arguments != 1) {
	//	LOG_WARNING("AddComponent function takes 1 argument(string)!");
	//	return 0;
	//}
	//if (lua_isstring(luaState, 1)) {
	//	string componentName = lua_tostring(luaState, 1);
	//	if (componentName == "Transform") {
	//		LOG_WARNING("Cannot add another Transform component!");
	//		return 0;
	//	}
	//	else if (componentName == "SpriteRender") {
	//		LOG_WARNING("Cannot add another SpriteRender component!");
	//		return 0;
	//	}
	//	//else if (componentName == )
	//}
	//else {
	//	LOG_WARNING("Argument of AddComponent function is not a string!");
	//	return 0;
	//}
	return 1;
}

int LuaScript::GetGameObjectLayer(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetLayer(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetLayer() GameObject argument is nil");
						return 0;
					}
					lua_pushstring(luaState, go->layer.c_str());
				}
				else {
					LOG_WARNING("GetLayer() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetLayer() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectLayer(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("SetLayer(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetLayer() GameObject argument is nil");
					}
					else {
						if (lua_isstring(luaState, 2)) {
							string layer = lua_tostring(luaState, 2);
							go->layer = layer;
						}
						else {
							LOG_WARNING("SetLayer() string argument is not a string!");
						}
					}
				}
				else {
					LOG_WARNING("SetLayer() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetLayer() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectName(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetName(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetName() GameObject argument is nil");
						return 0;
					}
					lua_pushstring(luaState, go->name.c_str());
				}
				else {
					LOG_WARNING("GetName() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetName() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectName(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("SetName(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetName() GameObject argument is nil");
					}
					else {
						if (lua_isstring(luaState, 2)) {
							string name = lua_tostring(luaState, 2);
							go->Rename(name);
							engine->sceneManagerModule->RenameDuplicatedGameObject(go);
						}
						else {
							LOG_WARNING("SetName() string argument is not a string!");
						}
					}
				}
				else {
					LOG_WARNING("SetName() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetName() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectTag(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetTag(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetTag() GameObject argument is nil");
						return 0;
					}
					lua_pushstring(luaState, go->tag.c_str());
				}
				else {
					LOG_WARNING("GetTag() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetTag() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectTag(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("SetTag(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetTag() GameObject argument is nil");
					}
					else {
						if (lua_isstring(luaState, 2)) {
							string tag = lua_tostring(luaState, 2);
							go->tag = tag;
						}
						else {
							LOG_WARNING("SetTag() string argument is not a string!");
						}
					}
				}
				else {
					LOG_WARNING("SetTag() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetTag() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectParent(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1 && arguments != 2) {
		LOG_WARNING("GetParent(GameObject, string) takes 1 or 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			if (arguments == 1) {
				lua_gettable(luaState, -2);
			}
			else {
				lua_gettable(luaState, -3);
			}
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					if (arguments == 1) {
						lua_gettable(luaState, -2);
					}
					else {
						lua_gettable(luaState, -3);
					}
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetParent() GameObject argument is nil!");
						return 0;
					}
					if (arguments == 1) {
						if (go->parent == nullptr) {
							LOG_WARNING("%s parent is nil!", go->name.c_str());
							return 0;
						}
						lua_newtable(luaState);
						lua_pushstring(luaState, "Type");
						lua_pushstring(luaState, "GameObject");
						lua_settable(luaState, -3);
						lua_pushstring(luaState, "Data");
						lua_pushlightuserdata(luaState, go->parent);
						lua_settable(luaState, -3);
					}
					else {
						if (lua_isstring(luaState, 2)) {
							string name = lua_tostring(luaState, 2);
							string goName = go->name;
							while (go->parent != nullptr) {
								if (go->parent->name == name) {
									lua_newtable(luaState);
									lua_pushstring(luaState, "Type");
									lua_pushstring(luaState, "GameObject");
									lua_settable(luaState, -3);
									lua_pushstring(luaState, "Data");
									lua_pushlightuserdata(luaState, go->parent);
									lua_settable(luaState, -3);
									return 1;
								}
								else {
									go = go->parent;
								}
							}
							LOG_WARNING("'%s' is not '%s' parent or doesn't exist in '%s' branch!", name.c_str(), goName.c_str(), goName.c_str());
							return 0;
						}
						else {
							LOG_WARNING("GetParent() string argument is not a string!");
							return 0;
						}
					}
				}
				else {
					LOG_WARNING("GetParent() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetParent() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectParent(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("SetParent(GameObject, GameObject) takes 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetParent() GameObject(1) argument is nil!");
					}
					else {
						lua_pop(luaState, 1);
						if (lua_istable(luaState, 2)) {
							lua_pushstring(luaState, "Type");
							lua_gettable(luaState, -2);
							if (lua_isstring(luaState, -1)) {
								string str = lua_tostring(luaState, -1);
								if (str == "GameObject")
								{
									lua_pop(luaState, 1);
									lua_pushstring(luaState, "Data");
									lua_gettable(luaState, -2);
									GameObject* parent = (GameObject*)lua_touserdata(luaState, -1);
									if (parent == nullptr) {
										LOG_WARNING("SetParent() GameObject(2) argument is nil!");
									}
									else {
										go->SetParent(parent);
									}
								}
								else {
									LOG_WARNING("SetParent() GameObject(2) argument is not a GameObject!");
								}
							}
						}
						else {
							LOG_WARNING("SetParent() GameObject(2) argument is not a GameObject!");
						}
					}
				}
				else {
					LOG_WARNING("SetParent() GameObject(1) argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetParent() GameObject(1) argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectRoot(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetRoot(GameObject) takes 1 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetRoot() GameObject argument is nil!");
						return 0;
					}
					while (go->parent != nullptr) {
						go = go->parent;
					}
					lua_newtable(luaState);
					lua_pushstring(luaState, "Type");
					lua_pushstring(luaState, "GameObject");
					lua_settable(luaState, -3);
					lua_pushstring(luaState, "Data");
					lua_pushlightuserdata(luaState, go);
					lua_settable(luaState, -3);
				}
				else {
					LOG_WARNING("GetRoot() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetRoot() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::GetGameObjectChild(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("GetChild(GameObject, string) takes 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetChild() GameObject argument is nil!");
						return 0;
					}
					if (lua_isstring(luaState, 2)) {
						string name = lua_tostring(luaState, 2);
						for (list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++) {
							if ((*it)->name == name) {
								lua_newtable(luaState);
								lua_pushstring(luaState, "Type");
								lua_pushstring(luaState, "GameObject");
								lua_settable(luaState, -3);
								lua_pushstring(luaState, "Data");
								lua_pushlightuserdata(luaState, (*it));
								lua_settable(luaState, -3);
								return 1;
							}
						}
						LOG_WARNING("'%s' is not '%s' child!", name.c_str(), go->name.c_str());
						return 0;
					}
					else {
						LOG_WARNING("GetChild() string argument is not a string!");
						return 0;
					}
				}
				else {
					LOG_WARNING("GetChild() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetChild() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::GetGameObjectChildByIndex(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("GetChild(GameObject, int) takes 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetChild() GameObject argument is nil!");
						return 0;
					}
					if (lua_isinteger(luaState, 2)) {
						int index = lua_tointeger(luaState, 2);
						if (index >= 0 && index < go->childs.size()) {
							list<GameObject*>::iterator it = go->childs.begin();
							for (int i = 0; i < index; i++) {
								it++;
							}
							if ((*it) == nullptr) {
								LOG_WARNING("'%s'child at index %d is nil!", go->name.c_str(), index);
								return 0;
							}
							lua_newtable(luaState);
							lua_pushstring(luaState, "Type");
							lua_pushstring(luaState, "GameObject");
							lua_settable(luaState, -3);
							lua_pushstring(luaState, "Data");
							lua_pushlightuserdata(luaState, (*it));
							lua_settable(luaState, -3);
						}
						else {
							LOG_WARNING("'%s' doesn't have %d childs!", go->name.c_str(), index);
							return 0;
						}
					}
					else {
						LOG_WARNING("GetChild() string argument is not a string!");
						return 0;
					}
				}
				else {
					LOG_WARNING("GetChild() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetChild() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::GetGameObjectChildsCount(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetChildsCount(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetChildsCount() GameObject argument is nil!");
						return 0;
					}
					lua_pushinteger(luaState, go->childs.size());
				}
				else {
					LOG_WARNING("GetChildsCount() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetChildsCount() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::FindGameObject(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("FindGameObject(string) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isstring(luaState, 1)) {
			string name = lua_tostring(luaState, 1);
			GameObject* go = engine->sceneManagerModule->FindGameObject(name);
			if (go == nullptr) {
				LOG_WARNING("%s was not found in the scene!", name.c_str());
				return 0;
			}
			else {
				lua_newtable(luaState);
				lua_pushstring(luaState, "Type");
				lua_pushstring(luaState, "GameObject");
				lua_settable(luaState, -3);
				lua_pushstring(luaState, "Data");
				lua_pushlightuserdata(luaState, go);
				lua_settable(luaState, -3);
			}
		}
		else {
			LOG_WARNING("FindGameObject() string argument is not a string!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::DestroyGameObject(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2 && arguments != 1) {
		LOG_WARNING("Destroy(GameObject, float) takes 1 or 2 arguments(float is optional)!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			if (arguments == 1) {
				lua_gettable(luaState, -2);
			}
			else {
				lua_gettable(luaState, -3);
			}
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					if (arguments == 1) {
						lua_gettable(luaState, -2);
					}
					else {
						lua_gettable(luaState, -3);
					}
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("Destroy() GameObject argument is nil");
					}
					else {
						if (arguments == 1) {
							go->Destroy();
						}
						else {
							if (lua_isnumber(luaState, 2)) {
								float time = lua_tonumber(luaState, 2);
								go->DestroyAfterTime(time);
							}
							else {
								LOG_WARNING("Destroy() float argument is not a number!");
							}
						}
					}
				}
				else {
					LOG_WARNING("Destroy() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("Destroy() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::Invoke(lua_State * luaState)
{
	if (insideFunction) {
		int arguments = lua_gettop(luaState);
		if (arguments != 2) {
			LOG_WARNING("Invoke(string, float) takes 2 arguments!");
		}
		else {
			if (lua_isstring(luaState, 1)) {
				string function = lua_tostring(luaState, 1);
				if (LuaUtils::FunctionExist(luaState, function.c_str())) {
					if (lua_isnumber(luaState, 2)) {
						float time = lua_tonumber(luaState, 2);
						if (time = 0) {
							LuaUtils::CallFunction(luaState, function.c_str());
						}
						else {
							//call after time
						}
					}
					else {
						LOG_WARNING("Invoke() float argument is not a number!");
					}
				}
				else {
					LOG_WARNING("Function %s doesn't exist in the script!", function.c_str());
				}
			}
			else {
				LOG_WARNING("Invoke() string argument is not a string!");
			}
		}
	}
	else {
		LOG_WARNING("Invoke cannot be called outside functions!");
	}
	return 0;
}

int LuaScript::Instantiate(lua_State * luaState)
{
	if (insideFunction) {
		int arguments = lua_gettop(luaState);
		if (arguments != 1) {
			LOG_WARNING("Instantiate(GameObject) takes 1 argument!");
			return 0;
		}
		else {
			GameObject* go = nullptr;
			GameObject* duplicatedGO = nullptr;
			if (lua_istable(luaState, 1)) {
				lua_pushstring(luaState, "Type");
				lua_gettable(luaState, -2);
				if (lua_isstring(luaState, -1)) {
					string str = lua_tostring(luaState, -1);
					if (str == "GameObject")
					{
						lua_pop(luaState, 1);
						lua_pushstring(luaState, "Data");
						lua_gettable(luaState, -2);
						go = (GameObject*)lua_touserdata(luaState, -1);
						if (go == nullptr) {
							LOG_WARNING("Instantiate() GameObject argument is nil!");
							return 0;
						}
						else {
							duplicatedGO = engine->sceneManagerModule->DuplicateGameObject(go);
							if (duplicatedGO == nullptr) {
								LOG_WARNING("Failed to Instantiate %s!", go->name.c_str());
								return 0;
							}
							else {
								lua_newtable(luaState);
								lua_pushstring(luaState, "Type");
								lua_pushstring(luaState, "GameObject");
								lua_settable(luaState, -3);
								lua_pushstring(luaState, "Data");
								lua_pushlightuserdata(luaState, duplicatedGO);
								lua_settable(luaState, -3);
							}
						}
					}
					else {
						LOG_WARNING("Instantiate() cannot be called outside functions!");
						return 0;
					}
				}
			}
		}
	}
	else {
		LOG_WARNING("Instantiate() cannot be called outside Main functions!");
		return 0;
	}
	return 1;
}

int LuaScript::FlipGameObjectSpriteX(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("FlipSpriteX(GameObject) takes 1 argument!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("FlipSpriteX() GameObject argument is nil!");
					}
					else {
						if (go->gameObjectSprite->getTextureRect().left == 0) {
							go->gameObjectSprite->setTextureRect(sf::IntRect(go->gameObjectSprite->getTextureRect().width, 
								go->gameObjectSprite->getTextureRect().top != 0 ? go->gameObjectSprite->getTextureRect().top : 0, 
								-go->gameObjectSprite->getTextureRect().width, go->gameObjectSprite->getTextureRect().height));
						}
						else {
							go->gameObjectSprite->setTextureRect(sf::IntRect(0, go->gameObjectSprite->getTextureRect().top != 0 ? go->gameObjectSprite->getTextureRect().top : 0, 
								-go->gameObjectSprite->getTextureRect().width, go->gameObjectSprite->getTextureRect().height));
						}
					}
				}
				else {
					LOG_WARNING("FlipSpriteX() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("FlipSpriteX() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::FlipGameObjectSpriteY(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("FlipSpriteY(GameObject) takes 1 argument!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("FlipSpriteY() GameObject argument is nil!");
					}
					else {
						if (go->gameObjectSprite->getTextureRect().top == 0) {
							go->gameObjectSprite->setTextureRect(sf::IntRect(go->gameObjectSprite->getTextureRect().left != 0 ? go->gameObjectSprite->getTextureRect().left : 0, 
								go->gameObjectSprite->getTextureRect().height, go->gameObjectSprite->getTextureRect().width, -go->gameObjectSprite->getTextureRect().height));
						}
						else {
							go->gameObjectSprite->setTextureRect(sf::IntRect(go->gameObjectSprite->getTextureRect().left != 0 ? go->gameObjectSprite->getTextureRect().left : 0, 0, 
								go->gameObjectSprite->getTextureRect().width, -go->gameObjectSprite->getTextureRect().height));
						}
					}
				}
				else {
					LOG_WARNING("FlipY() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("FlipY() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::TintGameObjectSprite(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("TintSprite(GameObject, Vector4) takes 2 argument!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("TintSprite() GameObject argument is nil!");
					}
					else {
						lua_pop(luaState, 1);;
						if (lua_istable(luaState, 2)) {
							if (lua_rawlen(luaState, 2) == 4) {
								int r, g, b, a;

								lua_pushinteger(luaState, 1);
								lua_gettable(luaState, -2);
								if (lua_isnumber(luaState, -1)) {
									if (lua_tonumber(luaState, -1) >= 0 && lua_tonumber(luaState, -1) <= 255) {
										r = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("TintSprite() value 'r' is not between 0 and 255!");
										return 0;
									}
								}
								else {
									LOG_WARNING("TintSprite() value 'r' is not a number!");
									return 0;
								}
								lua_pop(luaState, 1);

								lua_pushinteger(luaState, 2);
								lua_gettable(luaState, -2);
								if (lua_isnumber(luaState, -1)) {
									if (lua_tonumber(luaState, -1) >= 0 && lua_tonumber(luaState, -1) <= 255) {
										g = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("TintSprite() value 'g' is not between 0 and 255!");
										return 0;
									}
								}
								else {
									LOG_WARNING("TintSprite() value 'g' is not a number!");
									return 0;
								}
								lua_pop(luaState, 1);

								lua_pushinteger(luaState, 3);
								lua_gettable(luaState, -2);
								if (lua_isnumber(luaState, -1)) {
									if (lua_tonumber(luaState, -1) >= 0 && lua_tonumber(luaState, -1) <= 255) {
										b = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("TintSprite() value 'b' is not between 0 and 255!");
										return 0;
									}
								}
								else {
									LOG_WARNING("TintSprite() value 'b' is not a number!");
									return 0;
								}
								lua_pop(luaState, 1);

								lua_pushinteger(luaState, 4);
								lua_gettable(luaState, -2);
								if (lua_isnumber(luaState, -1)) {
									if (lua_tonumber(luaState, -1) >= 0 && lua_tonumber(luaState, -1) <= 255) {
										a = lua_tonumber(luaState, -1);
									}
									else {
										LOG_WARNING("TintSprite() value 'a' is not between 0 and 255!");
										return 0;
									}
								}
								else {
									LOG_WARNING("TintSprite() value 'a' is not a number!");
									return 0;
								}
								lua_pop(luaState, 1);
								sf::Color color(r, g, b, a);
								go->gameObjectSprite->setColor(color);
							}
							else {
								LOG_WARNING("TintSprite() Vector4 argument is not a Vector4!");
							}
						}
					}
				}
				else {
					LOG_WARNING("TintSprite() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("TintSprite() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectSpriteLocalBounds(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetSpriteLocalBounds(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetSpriteLocalBounds() GameObject argument is nil!");
						return 0;
					}
					else {
						lua_newtable(luaState);
						lua_pushnumber(luaState, go->gameObjectSprite->getLocalBounds().left);
						lua_rawseti(luaState, -2, 1);
						lua_pushnumber(luaState, go->gameObjectSprite->getLocalBounds().top);
						lua_rawseti(luaState, -2, 2);
						lua_pushnumber(luaState, go->gameObjectSprite->getLocalBounds().width);
						lua_rawseti(luaState, -2, 3);
						lua_pushnumber(luaState, go->gameObjectSprite->getLocalBounds().height);
						lua_rawseti(luaState, -2, 4);
					}
				}
				else {
					LOG_WARNING("GetSpriteLocalBounds() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetSpriteLocalBounds() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::GetGameObjectSpriteGlobalBounds(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("GetSpriteGlobalBounds(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetSpriteGlobalBounds() GameObject argument is nil!");
						return 0;
					}
					else {
						lua_newtable(luaState);
						lua_pushnumber(luaState, go->gameObjectSprite->getGlobalBounds().left);
						lua_rawseti(luaState, -2, 1);
						lua_pushnumber(luaState, go->gameObjectSprite->getGlobalBounds().top);
						lua_rawseti(luaState, -2, 2);
						lua_pushnumber(luaState, go->gameObjectSprite->getGlobalBounds().width);
						lua_rawseti(luaState, -2, 3);
						lua_pushnumber(luaState, go->gameObjectSprite->getGlobalBounds().height);
						lua_rawseti(luaState, -2, 4);
					}
				}
				else {
					LOG_WARNING("GetSpriteGlobalBounds() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetSpriteGlobalBounds() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

/////////////// Math //////////////////

int LuaScript::Vector4(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 4 && arguments != 0) {
		LOG_WARNING("Vector4 takes 4 numbers or can be empty");
		return 0;
	}
	else {
		if (arguments == 4) {
			for (int i = 1; i <= arguments; i++) {
				if (!lua_isnumber(luaState, i)) {
					LOG_WARNING("Argument %d in Vector4 is not a number!", i);
					return 0;
				}
			}
			lua_newtable(luaState);
			lua_pushnumber(luaState, lua_tonumber(luaState, 1));
			lua_rawseti(luaState, -2, 1);
			lua_pushnumber(luaState, lua_tonumber(luaState, 2));
			lua_rawseti(luaState, -2, 2);
			lua_pushnumber(luaState, lua_tonumber(luaState, 3));
			lua_rawseti(luaState, -2, 3);
			lua_pushnumber(luaState, lua_tonumber(luaState, 4));
			lua_rawseti(luaState, -2, 4);
		}
		else {
			lua_newtable(luaState);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 1);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 2);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 3);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 4);
		}
	}
	return 1;
}

int LuaScript::Vector3(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 3 && arguments != 0) {
		LOG_WARNING("Vector3 takes 3 numbers or can be empty");
		return 0;
	}
	else {
		if (arguments == 3) {
			for (int i = 1; i <= arguments; i++) {
				if (!lua_isnumber(luaState, i)) {
					LOG_WARNING("Argument %d in Vector3 is not a number!", i);
					return 0;
				}
			}
			lua_newtable(luaState);
			lua_pushnumber(luaState, lua_tonumber(luaState, 1));
			lua_rawseti(luaState, -2, 1);
			lua_pushnumber(luaState, lua_tonumber(luaState, 2));
			lua_rawseti(luaState, -2, 2);
			lua_pushnumber(luaState, lua_tonumber(luaState, 3));
			lua_rawseti(luaState, -2, 3);
		}
		else {
			lua_newtable(luaState);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 1);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 2);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 3);
		}
	}
	return 1;
}

int LuaScript::Vector2(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2 && arguments != 0) {
		LOG_WARNING("Vector2 takes 2 numbers or can be empty");
		return 0;
	}
	else {
		if (arguments == 2) {
			for (int i = 1; i <= arguments; i++) {
				if (!lua_isnumber(luaState, i)) {
					LOG_WARNING("Argument %d in Vector2 is not a number!", i);
					return 0;
				}
			}
			lua_newtable(luaState);
			lua_pushnumber(luaState, lua_tonumber(luaState, 1));
			lua_rawseti(luaState, -2, 1);
			lua_pushnumber(luaState, lua_tonumber(luaState, 2));
			lua_rawseti(luaState, -2, 2);
		}
		else {
			lua_newtable(luaState);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 1);
			lua_pushnumber(luaState, 0);
			lua_rawseti(luaState, -2, 2);
		}
	}
	return 1;
}

/////////////// Input  //////////////////

int LuaScript::IsKeyPressed(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsKeyPressed(string) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isstring(luaState, 1)) {
			string str = lua_tostring(luaState, 1);
			sf::Keyboard::Key key = engine->inputModule->StringToKey(str);
			if (key != -1) {
				lua_pushboolean(luaState, (int)engine->inputModule->IsKeyPressed(key));
			}
			else {
				LOG_WARNING("IsKeyPressed() string argument is not a valid key!");
				return 0;
			}
		}
		else {
			LOG_WARNING("IsKeyPressed() string argument is not a string!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::IsKeyReleased(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsKeyReleased(string) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isstring(luaState, 1)) {
			string str = lua_tostring(luaState, 1);
			sf::Keyboard::Key key = engine->inputModule->StringToKey(str);
			if (key != -1) {
				lua_pushboolean(luaState, (int)engine->inputModule->IsKeyReleased(key));
			}
			else {
				LOG_WARNING("IsKeyReleased() string argument is not a valid key!");
				return 0;
			}
		}
		else {
			LOG_WARNING("IsKeyReleased() string argument is not a string!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::IsKeyRepeated(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsKeyRepeated(string) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isstring(luaState, 1)) {
			string str = lua_tostring(luaState, 1);
			sf::Keyboard::Key key = engine->inputModule->StringToKey(str);
			if (key != -1) {
				lua_pushboolean(luaState, (int)engine->inputModule->IsKeyRepeated(key));
			}
			else {
				LOG_WARNING("IsKeyRepeated() string argument is not a valid key!");
				return 0;
			}
		}
		else {
			LOG_WARNING("IsKeyRepeated() string argument is not a string!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::IsMouseButtonPressed(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsMouseButtonPressed(int) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isinteger(luaState, 1)) {
			sf::Mouse::Button button = (sf::Mouse::Button)lua_tointeger(luaState, 1);
			if (button >= 0 && button <= 4) {
				lua_pushboolean(luaState, (int)engine->inputModule->IsMouseButtonPressed(button));
			}
			else {
				LOG_WARNING("IsMouseButtonPressed() integer argument is not a valid number!");
				return 0;
			}
		}
		else {
			LOG_WARNING("IsMouseButtonPressed() integer argument is not a number!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::IsMouseButtonReleased(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsMouseButtonReleased(int) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isinteger(luaState, 1)) {
			sf::Mouse::Button button = (sf::Mouse::Button)lua_tointeger(luaState, 1);
			if (button >= 0 && button <= 4) {
				lua_pushboolean(luaState, (int)engine->inputModule->IsMouseButtonReleased(button));
			}
			else {
				LOG_WARNING("IsMouseButtonReleased() integer argument is not a valid number!");
				return 0;
			}
		}
		else {
			LOG_WARNING("IsMouseButtonReleased() integer argument is not a number!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::IsMouseButtonRepeated(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsMouseButtonRepeated(int) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_isinteger(luaState, 1)) {
			sf::Mouse::Button button = (sf::Mouse::Button)lua_tointeger(luaState, 1);
			if (button >= 0 && button <= 4) {
				lua_pushboolean(luaState, (int)engine->inputModule->IsMouseButtonRepeated(button));
			}
			else {
				LOG_WARNING("IsMouseButtonRepeated() integer argument is not a valid number!");
				return 0;
			}
		}
		else {
			LOG_WARNING("IsMouseButtonRepeated() integer argument is not a number!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::IsGameObjectAnimationPlaying(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("IsAnimationPlaying(GameObject, string) takes 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("IsAnimationPlaying() GameObject argument is nil");
						return 0;
					}
					ComponentAnimation* animation = (ComponentAnimation*)go->GetComponent(Component::Animaton);
					if (animation == nullptr) {
						LOG_ERROR("IsAnimationPlaying() failed to get component Animation");
						return 0;
					}
					else {
						if (lua_isstring(luaState, 2)) {
							string animName = lua_tostring(luaState, 2);
							if (animation->GetPlayingAnimationName() == animName) {
								lua_pushboolean(luaState, true);
							}
						}
					}
				}
				else {
					LOG_WARNING("IsAnimationPlaying() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("IsAnimationPlaying() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::PlayGameObjectAnimation(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("PlayAnimation(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PlayAnimation() GameObject argument is nil");
					}
					else {
						ComponentAnimation* animation = (ComponentAnimation*)go->GetComponent(Component::Animaton);
						if (animation == nullptr) {
							LOG_ERROR("PlayAnimation() failed to get component Animation");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string animName = lua_tostring(luaState, 2);
								ResourceAnimation* anim = animation->GetAnimation(animName);
								if (anim != nullptr) {
									animation->SetAnimation(anim);
									animation->Play();
								}
								else {
									LOG_WARNING("%s is not an animation of GameObject %s!", animName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("PlayAnimation() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("PlayAnimation() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PlayAnimation() GameObject argument is not a GameObject!");
		}
	}

	return 0;
}

int LuaScript::StopGameObjectAnimation(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("StopAnimation(GameObject) takes 1 argument!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("StopAnimation() GameObject argument is nil");
					}
					else {
						ComponentAnimation* animation = (ComponentAnimation*)go->GetComponent(Component::Animaton);
						if (animation == nullptr) {
							LOG_ERROR("StopAnimation() failed to get component Animation");
						}
						animation->Stop();
					}
				}
				else {
					LOG_WARNING("StopAnimation() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("StopAnimation() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::PauseGameObjectAnimation(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("PauseAnimation(GameObject) takes 1 argument!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PauseAnimation() GameObject argument is nil");
					}
					else {
						ComponentAnimation* animation = (ComponentAnimation*)go->GetComponent(Component::Animaton);
						if (animation == nullptr) {
							LOG_ERROR("PauseAnimation() failed to get component Animation");
						}
						animation->Pause();
					}
				}
				else {
					LOG_WARNING("PauseAnimation() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PauseAnimation() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::IsGameObjectAnimationFinished(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 1) {
		LOG_WARNING("IsAnimationFinished(GameObject) takes 1 argument!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -2);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -2);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("IsAnimationFinished() GameObject argument is nil");
						return 0;
					}
					ComponentAnimation* animation = (ComponentAnimation*)go->GetComponent(Component::Animaton);
					if (animation == nullptr) {
						LOG_ERROR("IsAnimationFinished() failed to get component Animation");
						return 0;
					}
					if (animation->GetAnimation() != nullptr) {
						lua_pushboolean(luaState, animation->IsFinish());
					}
					else {
						LOG_WARNING("There is no animation playing!");
						return 0;
					}
				}
				else {
					LOG_WARNING("IsAnimationFinished() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("IsAnimationFinished() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::ChangeGameObjectAnimationSpeed(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 3) {
		LOG_WARNING("ChangeAnimationSpeed(GameObject, string, float) takes 3 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -4);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -4);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("ChangeAnimationSpeed() GameObject argument is nil");
					}
					else {
						ComponentAnimation* animation = (ComponentAnimation*)go->GetComponent(Component::Animaton);
						if (animation == nullptr) {
							LOG_ERROR("ChangeAnimationSpeed() failed to get component Animation");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string animName = lua_tostring(luaState, 2);
								ResourceAnimation* anim = animation->GetAnimation(animName);
								if (anim != nullptr) {
									if (lua_isnumber(luaState, 3)) {
										float speed = lua_tonumber(luaState, 3);
										anim->GetAnimation()->fps = speed;
									}
									else {
										LOG_WARNING("ChangeAnimationSpeed() float argument is not a number!");
									}
								}
								else {
									LOG_WARNING("%s is not an animation of GameObject %s!", animName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("ChangeAnimationSpeed() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("ChangeAnimationSpeed() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("ChangeAnimationSpeed() GameObject argument is not a GameObject!");
		}
	}

	return 0;
}

int LuaScript::PlayGameObjectSound(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("PlaySound(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PlaySound() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("PlaySound() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string soundName = lua_tostring(luaState, 2);
								if (audio->SoundExist(soundName)) {
									audio->PlaySound(soundName);
								}
								else {
									LOG_WARNING("%s is not a Sound of GameObject %s!", soundName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("PlaySound() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("PlaySound() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PlaySound() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::StopGameObjectSound(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("StopSound(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("StopSound() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("StopSound() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string soundName = lua_tostring(luaState, 2);
								if (audio->SoundExist(soundName)) {
									audio->StopSound(soundName);
								}
								else {
									LOG_WARNING("%s is not a Sound of GameObject %s!", soundName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("StopSound() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("StopSound() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("StopSound() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::PauseGameObjectSound(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("PauseSound(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PauseSound() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("PauseSound() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string soundName = lua_tostring(luaState, 2);
								if (audio->SoundExist(soundName)) {
									audio->PauseSound(soundName);
								}
								else {
									LOG_WARNING("%s is not a Sound of GameObject %s!", soundName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("PauseSound() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("PauseSound() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PauseSound() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectSoundState(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("GetSoundState(GameObject, string) takes 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetSoundState() GameObject argument is nil");
						return 0;
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("GetSoundState() failed to get component Audio");
							return 0;
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string soundName = lua_tostring(luaState, 2);
								if (audio->SoundExist(soundName)) {
									string state = audio->GetSoundState(soundName);
									lua_pushstring(luaState, state.c_str());
								}
								else {
									LOG_WARNING("%s is not a Sound of GameObject %s!", soundName.c_str(), go->name.c_str());
									return 0;
								}
							}
							else {
								LOG_WARNING("GetSoundState() string argument is not a string!");
								return 0;
							}
						}
					}
				}
				else {
					LOG_WARNING("GetSoundState() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetSoundState() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectSoundVolume(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 3) {
		LOG_WARNING("SetSoundVolume(GameObject, string, float) takes 3 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -4);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -4);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetSoundVolume() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("SetSoundVolume() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string soundName = lua_tostring(luaState, 2);
								if (audio->SoundExist(soundName)) {
									if (lua_isnumber(luaState, 3)) {
										float volume = lua_tonumber(luaState, 3);
										if (volume > 0 && volume <= 100) {
											audio->SetSoundVolume(soundName, volume);
										}
										else {
											LOG_WARNING("Volume value must be between 0 and 100, both included!");
										}
									}
									else {
										LOG_WARNING("SetSoundVolume() float argument is not a number!");
									}
								}
								else {
									LOG_WARNING("%s is not a Sound of GameObject %s!", soundName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("SetSoundVolume() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("SetSoundVolume() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetSoundVolume() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::SetGameObjectSoundPitch(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 3) {
		LOG_WARNING("SetSoundPitch(GameObject, string, float) takes 3 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -4);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -4);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetSoundPitch() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("SetSoundPitch() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string soundName = lua_tostring(luaState, 2);
								if (audio->SoundExist(soundName)) {
									if (lua_isnumber(luaState, 3)) {
										float pitch = lua_tonumber(luaState, 3);
										if (pitch > 0 && pitch <= 100) {
											audio->SetSoundPitch(soundName, pitch);
										}
										else {
											LOG_WARNING("Pitch value must be between 0 and 100, both included!");
										}
									}
									else {
										LOG_WARNING("SetSoundPitch() float argument is not a number!");
									}
								}
								else {
									LOG_WARNING("%s is not a Sound of GameObject %s!", soundName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("SetSoundPitch() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("SetSoundPitch() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetSoundPitch() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::PlayGameObjectMusic(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("PlayMusic(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PlayMusic() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("PlayMusic() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string musicName = lua_tostring(luaState, 2);
								if (audio->MusicExist(musicName)) {
									audio->PlayMusic(musicName);
								}
								else {
									LOG_WARNING("%s is not a Music of GameObject %s!", musicName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("PlayMusic() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("PlayMusic() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PlayMusic() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::StopGameObjectMusic(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("StopMusic(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("StopMusic() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("StopMusic() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string musicName = lua_tostring(luaState, 2);
								if (audio->MusicExist(musicName)) {
									audio->StopMusic(musicName);
								}
								else {
									LOG_WARNING("%s is not a Music of GameObject %s!", musicName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("StopMusic() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("StopMusic() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("StopMusic() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::PauseGameObjectMusic(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("PauseMusic(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PauseMusic() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("PauseMusic() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string musicName = lua_tostring(luaState, 2);
								if (audio->MusicExist(musicName)) {
									audio->PauseMusic(musicName);
								}
								else {
									LOG_WARNING("%s is not a Music of GameObject %s!", musicName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("PauseMusic() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("PauseMusic() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PauseMusic() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::GetGameObjectMusicState(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("GetMusicState(GameObject, string) takes 2 arguments!");
		return 0;
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("GetMusicState() GameObject argument is nil");
						return 0;
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("GetMusicState() failed to get component Audio");
							return 0;
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string musicName = lua_tostring(luaState, 2);
								if (audio->MusicExist(musicName)) {
									string state = audio->GetMusicState(musicName);
									lua_pushstring(luaState, state.c_str());
								}
								else {
									LOG_WARNING("%s is not a Music of GameObject %s!", musicName.c_str(), go->name.c_str());
									return 0;
								}
							}
							else {
								LOG_WARNING("GetMusicState() string argument is not a string!");
								return 0;
							}
						}
					}
				}
				else {
					LOG_WARNING("GetMusicState() GameObject argument is not a GameObject!");
					return 0;
				}
			}
		}
		else {
			LOG_WARNING("GetMusicState() GameObject argument is not a GameObject!");
			return 0;
		}
	}
	return 1;
}

int LuaScript::SetGameObjectMusicVolume(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 3) {
		LOG_WARNING("SetMusicVolume(GameObject, string, float) takes 3 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -4);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -4);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetMusicVolume() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("SetMusicVolume() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string musicName = lua_tostring(luaState, 2);
								if (audio->MusicExist(musicName)) {
									if (lua_isnumber(luaState, 3)) {
										float volume = lua_tonumber(luaState, 3);
										if (volume > 0 && volume <= 100) {
											audio->SetMusicVolume(musicName, volume);
										}
										else {
											LOG_WARNING("Volume value must be between 0 and 100, both included!");
										}
									}
									else {
										LOG_WARNING("SetMusicVolume() float argument is not a number!");
									}
								}
								else {
									LOG_WARNING("%s is not a Music of GameObject %s!", musicName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("SetMusicVolume() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("SetMusicVolume() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetMusicVolume() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::SetGameObjectMusicPitch(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 3) {
		LOG_WARNING("SetMusicPitch(GameObject, string, float) takes 3 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -4);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -4);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("SetMusicPitch() GameObject argument is nil");
					}
					else {
						ComponentAudio* audio = (ComponentAudio*)go->GetComponent(Component::AudioSource);
						if (audio == nullptr) {
							LOG_ERROR("SetMusicPitch() failed to get component Audio");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string musicName = lua_tostring(luaState, 2);
								if (audio->MusicExist(musicName)) {
									if (lua_isnumber(luaState, 3)) {
										float pitch = lua_tonumber(luaState, 3);
										if (pitch > 0 && pitch <= 100) {
											audio->SetMusicVolume(musicName, pitch);
										}
										else {
											LOG_WARNING("Pitch value must be between 0 and 100, both included!");
										}
									}
									else {
										LOG_WARNING("SetMusicPitch() float argument is not a number!");
									}
								}
								else {
									LOG_WARNING("%s is not a Music of GameObject %s!", musicName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("SetMusicPitch() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("SetMusicPitch() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("SetMusicPitch() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::PlayGameObjectParticleFX(lua_State * luaState)
{
	int arguments = lua_gettop(luaState);
	if (arguments != 2) {
		LOG_WARNING("PlayParticleFX(GameObject, string) takes 2 arguments!");
	}
	else {
		if (lua_istable(luaState, 1)) {
			lua_pushstring(luaState, "Type");
			lua_gettable(luaState, -3);
			if (lua_isstring(luaState, -1)) {
				string str = lua_tostring(luaState, -1);
				if (str == "GameObject")
				{
					lua_pop(luaState, 1);
					lua_pushstring(luaState, "Data");
					lua_gettable(luaState, -3);
					GameObject* go = (GameObject*)lua_touserdata(luaState, -1);
					if (go == nullptr) {
						LOG_WARNING("PlayParticleFX() GameObject argument is nil");
					}
					else {
						ComponentParticle* particleSystem = (ComponentParticle*)go->GetComponent(Component::ParticleSystem);
						if (particleSystem == nullptr) {
							LOG_ERROR("PlayParticleFX() failed to get component Particle System");
						}
						else {
							if (lua_isstring(luaState, 2)) {
								string particleFXName = lua_tostring(luaState, 2);
								ResourceAnimation* anim = particleSystem->GetParticleFXList;
								if (anim != nullptr) {
									animation->SetAnimation(anim);
									animation->Play();
								}
								else {
									LOG_WARNING("%s is not a particle effect of GameObject %s!", animName.c_str(), go->name.c_str());
								}
							}
							else {
								LOG_WARNING("PlayParticleFX() string argument is not a string!");
							}
						}
					}
				}
				else {
					LOG_WARNING("PlayParticleFX() GameObject argument is not a GameObject!");
				}
			}
		}
		else {
			LOG_WARNING("PlayParticleFX() GameObject argument is not a GameObject!");
		}
	}
	return 0;
}

int LuaScript::StopGameObjectParticleFX(lua_State * luaState)
{
	return 0;
}

int LuaScript::PauseGameObjectParticleFX(lua_State * luaState)
{
	return 0;
}


/////////////// Lua Utils //////////////////

void LuaUtils::RegisterLibrary(lua_State * luaState, luaL_Reg lib[], const char* libName)
{
	lua_getglobal(luaState, libName);
	if (lua_isnil(luaState, -1))
	{
		lua_pop(luaState, 1);
		lua_newtable(luaState);
	}
	luaL_setfuncs(luaState, lib, 0);
	lua_setglobal(luaState, libName);
}

bool LuaUtils::CallFunction(lua_State * luaState, const char* functionName)
{
	bool ret = true;

	if (functionName != NULL) {
		lua_getglobal(luaState, functionName);
	}

	int numArgs = 0;
	int numResults = 0;
	if (lua_pcall(luaState, numArgs, numResults, 0) != 0) {
		LOG_WARNING("Cannot call Lua function %s: %s", functionName, lua_tostring(luaState, -1));
		ret = false;
	}
	
	return ret;
}

bool LuaUtils::FunctionExist(lua_State * luaState, const char* functionName)
{
	bool ret = true;

	lua_getglobal(luaState, functionName);
	if (!lua_isfunction(luaState, lua_gettop(luaState))) {
		ret = false;
	}
	lua_pop(luaState, 1);

	return ret;
}

void LuaScript::StackDump(lua_State* l)
{
   int i;
   int top = lua_gettop(l);

   printf("total in stack %d\n", top);

   for (i = 1; i <= top; i++)
   {  /* repeat for each level */
		int t = lua_type(l, i);
		switch (t) {
			case LUA_TSTRING:  /* strings */
				printf("string: '%s'\n", lua_tostring(l, i));
				break;
			case LUA_TBOOLEAN:  /* booleans */
				printf("boolean %s\n", lua_toboolean(l, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:  /* numbers */
				printf("number: %g\n", lua_tonumber(l, i));
				break;
			case LUA_TTABLE:
				printf("table\n");
				/*while (lua_next(luaState, -2)) {

				}*/
				break;
			default:  /* other values */
			printf("%s\n", lua_typename(l, t));
			break;
		}
	}
	printf("\n");  /* end the listing */
}