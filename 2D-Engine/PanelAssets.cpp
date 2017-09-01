#include "PanelAssets.h"
#include "ModuleEditor.h"
#include "tinyfiledialogs.h"
#include "ModuleInput.h"
#include "ModuleSceneWindow.h"
#include "ModuleSceneManager.h"
#include "ModuleResources.h"
#include "ResourceAnimation.h"
#include "ResourceScript.h"
#include "ResourceSprite.h"
#include "ResourceMusic.h"
#include "ResourceSound.h"

PanelAssets::PanelAssets()
{
	active = true;
	panelName = "Assets";
	//folderImage = new sf::Texture();
	//folderImage->loadFromFile("../Data/folderImage.png");
	soundImage = new sf::Texture();
	soundImage->loadFromFile("../Data/speaker.png");
	textureImage = new sf::Texture();
	textureImage->loadFromFile("../Data/picture.png");
	luaScriptImage = new sf::Texture();
	luaScriptImage->loadFromFile("../Data/LuaScriptIcon.png");


	assetsPath = ASSETS_FOLDER;
	if (!fs::exists(assetsPath)) {
		if (fs::create_directory(ASSETS_FOLDER)) {
			assetsPath = ASSETS_FOLDER;
		}
		else {
			LOG_ERROR("Assets folder is not found and can't create new folder");
			return;
		}
	}
	selectedFolder = assetsPath;
	FillAssetsLists();
}


PanelAssets::~PanelAssets()
{
	delete folderImage;
}

void PanelAssets::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str(), false, false, engine->IsPlaying(), ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::Columns(2);
		node = 0;
		ImGui::Spacing();
		DrawChilds(assetsPath);

		if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
			ImGui::SetNextWindowPos(ImGui::GetMousePos());
			ImGui::OpenPopup("Assets Options");
		}

		if (!selectedFolder.empty()) {
			if (ImGui::BeginPopup("Assets Options"))
			{
				if (ImGui::MenuItem("Create Folder")) {
					showNewFolderWindow = true;
				}
				if (selectedFolder.filename().string() != "Assets") {
					if (ImGui::MenuItem("Delete")) {
						fs::remove_all(selectedFolder);
					}
				}
				/*if (ImGui::MenuItem("Create C++ Script")) {
					showNewScriptWindow = true;
					scriptType = hScript;
				}*/
				if (ImGui::MenuItem("Create Lua Script")) {
					showNewScriptWindow = true;
					scriptType = luaScript;
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Import Sprite")) {
					char const * lFilterPatterns[2] = { "*.jpg", "*.png" };
					const char* spritePath = tinyfd_openFileDialog("Select Sprite...", NULL, 2, lFilterPatterns, NULL, 0);
					if (spritePath != NULL) {
						fs::path oldPath(spritePath);
						fs::path newPath(selectedFolder.string() + "\\" + oldPath.filename().string());
						if (!fs::exists(newPath)) {
							if (oldPath != newPath) {
								fs::copy_file(oldPath, newPath);
							}
							else {
								tinyfd_messageBox("Error", "Open file name is NULL", "ok", "error", 1);
							}
						}
						else {
							tinyfd_messageBox("Error", "A file with this name exist in current folder", "ok", "error", 1);
						}
					}
				}
				ImGui::EndPopup();
			}
		}

		if (showNewFolderWindow && !showNewScriptWindow) {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("New Folder Name", &active,
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoTitleBar);
			ImGui::Spacing();
			ImGui::Text("New Folder Name");
			static char inputText[20];
			ImGui::InputText("", inputText, 20);
			ImGui::Spacing();
			if (ImGui::Button("Confirm")) {
				string str(inputText);
				fs::path temp = selectedFolder;
				if (fs::create_directory(selectedFolder += ("\\" + str))) {
					showNewFolderWindow = false;
				}
				else {
					selectedFolder = temp;
				}
				strcpy(inputText, "");
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				strcpy(inputText, "");
				showNewFolderWindow = false;
			}
			ImGui::End();
		}

		if (showNewScriptWindow && !showNewFolderWindow) {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("New Script Name", &active,
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoTitleBar);
			ImGui::Spacing();
			ImGui::Text("New Script Name");
			static char inputText[30];
			ImGui::InputText("", inputText, 30);
			ImGui::Spacing();
			if (ImGui::Button("Confirm")) {
				string str(inputText);
				if (!str.empty()) {
					str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
					CreateScript(hScript, inputText);
					strcpy(inputText, "");
					showNewScriptWindow = false;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				strcpy(inputText, "");
				showNewScriptWindow = false;
			}
			ImGui::End();
		}

		ImGui::NextColumn();

		//Need to optimize this!!
		if (ImGui::BeginChild("Files", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar, engine->IsPlaying())) {
			if (!selectedFolder.empty()) {
				for (auto & p : fs::directory_iterator(selectedFolder)) {
					if (!fs::is_directory(p)) {
						bool selected = false;
						switch (string2int(p.path().extension().string())) {
						case 0:
							ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
							ImGui::SameLine();
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
								}
							}
							break;
						case 1:
							//ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
							//ImGui::SameLine();
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
								}
							}
							break;
						case 2:
							ImGui::Image(*textureImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
							ImGui::SameLine();
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (!fileOptionsOpen && !engine->editorModule->dragData.hasData) {
									ImGui::BeginTooltip();
									if (toolTiptexture.loadFromFile(p.path().string())) {
										toolTiptexture.setSmooth(true);
										ImGui::Image(toolTiptexture, { 50,50 }, sf::Color::White, sf::Color::Transparent);
									}
									ImGui::EndTooltip();
								}
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
									else {
										if (!engine->editorModule->dragData.hasData) {
											engine->editorModule->dragData.fromPanel = "Files";
											engine->editorModule->dragData.path = p.path().string();
											engine->editorModule->dragData.extension = p.path().extension().string();
											engine->editorModule->dragData.name = p.path().filename().replace_extension().string();
											engine->editorModule->dragData.hasData = true;
											engine->editorModule->dragData.dragSprite.setTexture(toolTiptexture, true);
											engine->editorModule->dragData.dragSprite.setOrigin(engine->editorModule->dragData.dragSprite.getLocalBounds().width / 2, engine->editorModule->dragData.dragSprite.getLocalBounds().height / 2);
										}
									}
								}
							}
							break;
						case 3:
							ImGui::Image(*luaScriptImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
							ImGui::SameLine();
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
								}
							}
							break;
						case 4:
							//ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
							//ImGui::SameLine();
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
								}
							}
							break;
						case 5:
							//ImGui::Image(*soundImage, { 16,16 }, sf::Color::White, sf::Color::Transparent);
							//ImGui::SameLine();
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
								}
							}
							break;
						case 6:
							if (p.path() == selectedFilePath) {
								if (engine->sceneManagerModule->selectedGameObjects.empty()) {
									selected = true;
								}
								else {
									selectedFilePath.clear();
								}
							}
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemHoveredRect()) {
								if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
									selectedFilePath = p.path();
									engine->sceneManagerModule->selectedGameObjects.clear();
									if (ImGui::IsItemClicked(1)) {
										ImGui::SetNextWindowPos(ImGui::GetMousePos());
										ImGui::OpenPopup("File Options");
										fileOptionsOpen = true;
									}
									else {
										if (!engine->editorModule->dragData.hasData) {
											engine->editorModule->dragData.fromPanel = "Files";
											engine->editorModule->dragData.path = p.path().string();
											engine->editorModule->dragData.extension = p.path().extension().string();
											engine->editorModule->dragData.name = p.path().filename().replace_extension().string();
											engine->editorModule->dragData.hasData = true;
										}
									}
								}
							}
							break;
						default:
							if (p.path() == selectedFilePath) selected = true;
							ImGui::Selectable(p.path().filename().string().c_str(), &selected, 0, { 0,0 }, false);
							if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) && !fileOptionsOpen) {
								selectedFilePath = p.path();
								engine->sceneManagerModule->selectedGameObjects.clear();
								if (ImGui::IsItemClicked(1)) {
									ImGui::SetNextWindowPos(ImGui::GetMousePos());
									ImGui::OpenPopup("File Options");
									fileOptionsOpen = true;
								}
							}
							break;
						}
					}
				}
				if (ImGui::IsMouseHoveringWindow()) {
					if (ImGui::IsMouseReleased(0) && engine->editorModule->dragData.hasData) {
						if (engine->editorModule->dragData.fromPanel == "Hierarchy") {
							Data data;
							engine->editorModule->draggingGameObject->Save(data);
							data.AddInt("GameObjectsCount", engine->sceneManagerModule->savingIndex);
							data.SaveAsBinary(selectedFolder.string() + "\\" + engine->editorModule->draggingGameObject->name + ".prefab");
							engine->sceneManagerModule->savingIndex = 0;
							engine->editorModule->draggingGameObject = nullptr;
						}
						engine->editorModule->dragData.clearData();
					}
				}
			}

			if (ImGui::BeginPopup("File Options"))
			{
				if (ImGui::MenuItem("Rename")) {
					fileOptionsOpen = false;
				}
				if (ImGui::MenuItem("Delete")) {
					fs::remove(selectedFilePath);
					fileOptionsOpen = false;
				}

				ImGui::EndPopup();
			}
			else {
				fileOptionsOpen = false;
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndDock();
}

void PanelAssets::FillAssetsLists()
{
	for (auto & p : fs::recursive_directory_iterator(assetsPath)) {
		if (!fs::is_directory(p)) {
			if (p.path().extension().string() == ".h" || p.path().extension().string() == ".cs" || p.path().extension().string() == ".lua" || p.path().extension().string() == ".js") {
				ResourceScript* script = new ResourceScript(p.path().string(), p.path().filename().replace_extension("").string());
				engine->resourcesModule->AddResourceScript(script);
			}
			else if (p.path().extension().string() == ".png" || p.path().extension().string() == ".jpg") {
				ResourceSprite* sprite = new ResourceSprite(p.path().string(), p.path().filename().replace_extension("").string());
				engine->resourcesModule->AddResourceSprite(sprite);
			}
			else if (p.path().extension().string() == ".animation") {
				ResourceAnimation* animation = new ResourceAnimation(p.path().string(), p.path().filename().replace_extension("").string());
				engine->resourcesModule->AddResourceAnimation(animation);
			}
			else if (p.path().extension().string() == ".wav") {
				ResourceSound* sound = new ResourceSound(p.path().string(), p.path().filename().replace_extension("").string());
				engine->resourcesModule->AddResourceSound(sound);
			}
			else if (p.path().extension().string() == ".ogg") {
				ResourceMusic* music = new ResourceMusic(p.path().string(), p.path().filename().replace_extension("").string());
				engine->resourcesModule->AddResourceMusic(music);
			}
			else if (p.path().extension().string() == ".particleFX") {
				ResourceParticleEffect* particleFX = new ResourceParticleEffect(p.path().string(), p.path().filename().replace_extension("").string());
				engine->resourcesModule->AddResourceParticleFX(particleFX);
			}
		}
	}
}

void PanelAssets::DrawChilds(fs::path path)
{
	sprintf_s(nodeName, 30, "%s##node_%i", path.filename().string().c_str(), node++);
	uint flag = 0;

	if (fs::is_empty(path)) {
		flag |= ImGuiTreeNodeFlags_Leaf;
	}
	else {
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				flag = 0;
				break; //if fodler contains another folder, it can't be a leaf.
			}
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
	}

	flag |= ImGuiTreeNodeFlags_OpenOnArrow;

	if (selectedFolder == path && !showNewFolderWindow) {
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(nodeName, flag))
	{
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			selectedFolder = path;
		}
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				DrawChilds(p);
			}
		}
		ImGui::TreePop();
	}
	else {
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			selectedFolder = path;
		}
	}
}

void PanelAssets::CreateScript(ScripType type, string scriptName)
{
	ifstream inFile;
	string newFileName;

	switch (type) {
	case cppScript:
		inFile.open(CPP_TEMPLATE_FILE_PATH);
		newFileName = scriptName + ".cpp";
		break;
	case hScript:
		inFile.open(H_TEMPLATE_FILE_PATH);
		newFileName = scriptName + ".h";
		break;
	case csScript:
		newFileName = scriptName + ".cs";
		break;
	case jsScript:
		newFileName = scriptName + ".js";
		break;
	case luaScript:
		inFile.open(LUA_TEMPLATE_FILE_PATH);
		newFileName = scriptName + ".lua";
		break;
	}

	if (inFile.is_open()) {
		stringstream strStream;
		strStream << inFile.rdbuf();//read the file
		string str = strStream.str();//str holds the content of the file

		if (str.empty())
			return;
		size_t start_pos = 0;
		string headerNameTemplate = "#H_FILENAME#";
		if ((start_pos = str.find(headerNameTemplate, start_pos)) != std::string::npos) {
			str.replace(start_pos, headerNameTemplate.length(), scriptName);
		}
		start_pos = 0;
		string classNameTemplate = "#CLASS_NAME#";
		while ((start_pos = str.find(classNameTemplate, start_pos)) != std::string::npos) {
			str.replace(start_pos, classNameTemplate.length(), scriptName);
			start_pos += scriptName.length();
		}

		inFile.close();

		ofstream outputFile(selectedFolder.string() + "\\" + newFileName);
		outputFile << str;
		outputFile.close();

		ResourceScript* script = new ResourceScript(selectedFolder.string() + "\\" + newFileName, newFileName);
		engine->resourcesModule->GetGameScriptsList().push_back(script);

		if (type == hScript) {
			CreateScript(cppScript, scriptName);
		}
	}
	
}


int PanelAssets::string2int(string str)
{
	if (str == ".wav") return 0;
	else if (str == ".ogg") return 1;
	else if (str == ".jpg" || str == ".png") return 2;
	else if (str == ".lua") return 3;
	else if (str == ".animation") return 4;
	else if (str == ".particleFX") return 5;
	else if (str == ".prefab") return 6;
	else return -1;
}