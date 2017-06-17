#pragma once

#include <string>
#include <list>
#include "Globals.h"

using namespace std;

class Resource
{
public:
	enum ResourceType {
		spriteResource, sceneResource, animationResource, prefabResource, scriptResource, soundResource, musicResource, unknown
	};
public:
	Resource();
	virtual ~Resource();

	Resource::ResourceType GetType() const;
	string GetName() const;
	string GetPath() const;

public:
	ResourceType resourceType = unknown;
	string name;
	string path;
	UID uid = 0;
};

