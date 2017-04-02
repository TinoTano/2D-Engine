#pragma once

#include "Module.h"

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	virtual ~ModuleFileSystem();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	// Utility functions
	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	const char* GetSaveDirectory() const
	{
		return "save/";
	}

	unsigned int Load(const char* file, char** buffer) const;
	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;
};

