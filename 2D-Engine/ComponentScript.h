#pragma once
#include "Component.h"
#include "Script.h"
#include "LuaScript.h"

class ComponentScript : public Component
{
public:
	
	ComponentScript(GameObject* attachedObject);
	virtual ~ComponentScript();

	void InitScript(string sourceCode);

	void SetScriptName(string name);
	void SetScriptPath(string path);
	string GetScriptName() const;
	string GetScriptPath() const;
	LuaScript* GetScript() const;
	vector<ScriptField*> GetScriptFields() const;
	void SetScriptFields();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	string scriptName;
	LuaScript* luaScript = nullptr;
	string scriptPath;
	vector<ScriptField*> scriptFields;
};

