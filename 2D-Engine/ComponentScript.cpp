#include "ComponentScript.h"



ComponentScript::ComponentScript(GameObject* attachedObject)
{
	gameObject = attachedObject;
	type = Script;
}


ComponentScript::~ComponentScript()
{
}

void ComponentScript::InitScript(string sourceCode)
{
	luaScript = new LuaScript();
	luaScript->InitScript(sourceCode.c_str(), gameObject);
}

void ComponentScript::SetScriptName(string name)
{
	scriptName = name;
}

void ComponentScript::SetScriptPath(string path)
{
	scriptPath = path;
}

string ComponentScript::GetScriptName() const
{
	return scriptName;
}

string ComponentScript::GetScriptPath() const
{
	return scriptPath;
}

LuaScript * ComponentScript::GetScript() const
{
	return luaScript;
}

vector<ScriptField*> ComponentScript::GetScriptFields() const
{
	return scriptFields;
}

void ComponentScript::SetScriptFields()
{
	scriptFields = GetScript()->GetScriptFields();
}

void ComponentScript::Save(Data & data) const
{
	data.AddInt("Type", type);
	data.AddString("ScriptPath", scriptPath);
	data.AddString("ScriptName", scriptName);
}

void ComponentScript::Load(Data & data)
{
	scriptPath = data.GetString("ScriptPath");
	scriptName = data.GetString("ScriptName");
}
