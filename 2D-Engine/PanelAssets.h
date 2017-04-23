#pragma once

#include "Panel.h"
#include "Engine.h"
#include <experimental\filesystem>
namespace fs = experimental::filesystem;

struct ImVec3 { float x, y, z; ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };

class PanelAssets : public Panel
{
public:
	PanelAssets();
	virtual ~PanelAssets();

	void DrawPanel();
	void DrawChilds(fs::path path);

private:
	fs::path selectedFolder = "";
	uint node = 0;
	char nodeName[30];
	bool showNewFolderWindow = false;
	bool showDeleteAlert = false;
	sf::Texture* folderImage;
	sf::Texture* soundImage;
	sf::Texture* textureImage;
	string filePanel = "";
	ImVec4 color;
	bool draggingFile = false;
	sf::Sprite* spr;
};

