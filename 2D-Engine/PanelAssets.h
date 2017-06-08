#pragma once

#include "Panel.h"
#include "Engine.h"
#include <experimental\filesystem>
namespace fs = experimental::filesystem;

class PanelAssets : public Panel
{
public:
	enum ScripType {
		cppScript, hScript, csScript, jsScript, luaScript, unkwnow
	};

	PanelAssets();
	virtual ~PanelAssets();

	void DrawPanel();

private:
	void FillAssetsLists();
	void DrawChilds(fs::path path);
	void CreateScript(ScripType type, string scriptName);

private:
	fs::path selectedFolder;
	uint node = 0;
	char nodeName[30];
	bool showNewFolderWindow = false;
	sf::Texture* folderImage = nullptr;
	sf::Texture* soundImage = nullptr;
	sf::Texture* textureImage = nullptr;
	string filePanel;
	ImVec4 color;
	sf::Sprite* spr = nullptr;
	sf::Texture toolTiptexture;
	fs::path assetsPath;
	fs::path selectedFilePath;
	bool fileOptionsOpen = false;
	bool showNewScriptWindow = false;
	ScripType scriptType = unkwnow;
};

