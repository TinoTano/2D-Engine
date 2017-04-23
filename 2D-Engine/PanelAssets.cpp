#include "PanelAssets.h"
#include "ModuleEditor.h"
#include "tinyfiledialogs.h"
#include "ModuleInput.h"

PanelAssets::PanelAssets()
{
	active = true;
	panelName = "Assets";
	folderImage = new sf::Texture();
	folderImage->loadFromFile("../Data/folderImage.png");
	soundImage = new sf::Texture();
	soundImage->loadFromFile("../Data/speaker.png");
	textureImage = new sf::Texture();
	textureImage->loadFromFile("../Data/picture.png");
	spr = new sf::Sprite();
	spr->setTexture(*textureImage);
}


PanelAssets::~PanelAssets()
{
	delete folderImage;
}

void PanelAssets::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str())) {
		node = 0;
		fs::path path(ASSETS_FOLDER);
		ImGui::Spacing();
		DrawChilds(path);

		if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
			ImGui::OpenPopup("Assets Options");
		}

		if (!selectedFolder.empty()) {
			if (ImGui::BeginPopup("Assets Options"))
			{

				if (ImGui::MenuItem("Create Folder")) {
					showNewFolderWindow = true;
				}
				if (ImGui::MenuItem("Delete")) {
					showDeleteAlert = true;
				}
				if (ImGui::MenuItem("Create C++ Script")) {

				}
				ImGui::Separator();
				if (ImGui::MenuItem("Import Sprite")) {
					char const * lFilterPatterns[2] = { "*.jpg", "*.png" };
					const char* spritePath = tinyfd_openFileDialog("Select Sprite...", NULL, 2, lFilterPatterns, NULL, 0);
					if (spritePath != NULL) {
						fs::path oldPath(spritePath);
						fs::path newPath(selectedFolder.string() + "\\" + oldPath.filename().string());
						if (oldPath != newPath) {
							fs::copy_file(oldPath, newPath);
						}
						else {
							tinyfd_messageBox("Error","Open file name is NULL","ok","error",1);
						}
					}
				}
				ImGui::EndPopup();
			}
			if (engine->inputModule->IsKeyReleased(sf::Keyboard::Delete)) {
				showDeleteAlert = true;
			}
		}

		if (showNewFolderWindow && !showDeleteAlert) {
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

		if (showDeleteAlert && !showNewFolderWindow) {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, ImGui::GetWindowSize().y / 2));
			ImGui::SetNextWindowPosCenter();
			ImGui::Begin("Delete Alert", &active,
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoTitleBar);
			ImGui::Spacing();
			ImGui::Text("Delete %s?", selectedFolder.filename().string().c_str());
			ImGui::Spacing();
			if (ImGui::Button("Confirm")) {
				fs::remove_all(selectedFolder);
				showDeleteAlert = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				showDeleteAlert = false;
			}
			ImGui::End();
		}
	}
	ImGui::EndDock();

	////////////////////////
	if (ImGui::BeginDock("Files")) {
		if (!selectedFolder.empty()) {
			for (auto & p : fs::directory_iterator(selectedFolder)) {
				if (!fs::is_directory(p)) {
					if (p.path().extension().string() == ".wav") {
						ImGui::Image(*soundImage, { 13,13 }, sf::Color::White, sf::Color::Transparent);
						ImGui::SameLine();
					}
					else if (p.path().extension().string() == ".png" || p.path().extension().string() == ".jpg") {
						ImGui::Image(*textureImage, { 13,13 }, sf::Color::White, sf::Color::Transparent);
						ImGui::SameLine();
					}
					ImGui::Text("%s", p.path().filename().string().c_str());
					if (ImGui::IsItemHoveredRect()) {
						ImGui::BeginTooltip();
						ImGui::Image(*textureImage, sf::Color::White, sf::Color::Transparent);
						ImGui::EndTooltip();
						if (ImGui::IsItemClicked(0)) {
							draggingFile = true;
						}
					}
				}
			}
		}
	}
	ImGui::EndDock();
	
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

	if (selectedFolder == path && !showDeleteAlert && !showNewFolderWindow) {
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx((void*)folderImage->getNativeHandle(), { 16,12 }, nodeName, flag))
	{
		float folderNameSize = ImGui::CalcTextSize(path.filename().string().c_str()).x;
		
		if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImVec2(ImGui::GetItemRectMax().x + folderNameSize + 10, ImGui::GetItemRectMax().y)) && ImGui::IsMouseReleased(0)) {
			selectedFolder = path;
		}
		ImGui::Spacing();
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				DrawChilds(p);
			}
		}
		ImGui::TreePop();
	}
	else {
		float folderNameSize = ImGui::CalcTextSize(path.filename().string().c_str()).x;

		if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImVec2(ImGui::GetItemRectMax().x + folderNameSize + 10, ImGui::GetItemRectMax().y)) && ImGui::IsMouseReleased(0)) {
			selectedFolder = path;
		}
		ImGui::Spacing();
	}
}