#include "Resource.h"



Resource::Resource()
{
}


Resource::~Resource()
{
}

Resource::ResourceType Resource::GetType() const
{
	return resourceType;
}

string Resource::GetName() const
{
	return name;
}

string Resource::GetPath() const
{
	return path;
}
