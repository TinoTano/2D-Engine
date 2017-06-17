#include "ResourceScript.h"


ResourceScript::ResourceScript(string scriptPath, string name)
{
	this->script = scriptPath;
	this->name = name;
	resourceType = scriptResource;
	path = scriptPath;
	uid = ++uid;
}

ResourceScript::~ResourceScript()
{

}

string ResourceScript::GetScript() const
{
	return script;
}
