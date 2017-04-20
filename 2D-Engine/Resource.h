#pragma once

#include <string>
#include <list>

using namespace std;

class Resource
{
public:
	enum ResourceType {
		Texture, Scene, Audio, Animation, Prefab, Script
	};
public:
	Resource();
	virtual ~Resource();

	Resource::ResourceType GetType() const;

public:
	ResourceType resourceType;
	string name;
	list<Resource*> childs;
};

