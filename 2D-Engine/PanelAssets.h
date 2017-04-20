#pragma once

#include "Panel.h"
#include "Engine.h"
#include "Resource.h"
#include <experimental\filesystem>
namespace fs = experimental::filesystem;

class PanelAssets : public Panel
{
public:
	PanelAssets();
	virtual ~PanelAssets();

	void DrawPanel();
	void DrawChilds(fs::path path);
	void CheckMouseHover();

private:
	fs::path selectedFolder = "";
	uint node = 0;
	char nodeName[30];
};

