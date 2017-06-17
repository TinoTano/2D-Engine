#pragma once
#include "Resource.h"

class ResourceScript : public Resource
{
public:
	ResourceScript(string scriptPath, string name);
	virtual ~ResourceScript();

	string GetScript() const;

private:
	string script;
};

