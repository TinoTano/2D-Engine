#include "PanelAnimator.h"
#include "Engine.h"
#include "ModuleResources.h"
#include "tinyfiledialogs.h"
#include "Data.h"
#include <experimental\filesystem>
namespace fs = experimental::filesystem;


PanelAnimator::PanelAnimator()
{
	active = false;
	panelName = "Animator";
	animation = new Animation();
	isPlaying = false;
	isPaused = true;
	currentFrameIndex = 1;
	nameError = false;
}

PanelAnimator::~PanelAnimator()
{
	delete animation;
}

void PanelAnimator::DrawPanel()
{
	ImGui::SetNextWindowSize({ 500,700 });
	ImGui::Begin(panelName.c_str(), &active,
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::Text("Name");
	ImGui::SameLine(50);
	static char text[20];
	ImGui::InputText("##Name", text, 20);
	if (nameError) {
		ImGui::SameLine();
		ImGui::TextColored({ 1,0,0,1 }, "Invalid name!");
	}
	ImGui::Text("Speed");
	ImGui::SameLine(50);
	ImGui::InputFloat("##Speed", &animation->fps);
	ImGui::Text("Loop");
	ImGui::SameLine(50);
	ImGui::Checkbox("##Loop", &animation->loop);
	ImGui::Spacing();
	ImGui::Text("Frames");
	ImGui::Separator();

	if (animation->Size() == 0) {
		ImGui::Text("No frames");
	}
	else {
		for (int i = 0; i < animation->Size(); i++) {
			string frame = "Frame " + to_string(i + 1);
			ImGui::Text(frame.c_str());
			ImGui::SameLine();
			ImGui::Text(animation->frames[i]->GetName().c_str());
			ImGui::SameLine();
			if (ImGui::Button(("Remove##" + frame).c_str())) {
				animation->RemoveFrame(i);
			}
		}
	}
	
	ImGui::Spacing();
	
	if (ImGui::Button("Add sprite")) {
		engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, true);
	}

	if (animation->Size() > 0) {
		if (ImGui::Button("Save animation")) {
			string animName(text);
			if (animName.empty() || animName[0] == ' ') {
				nameError = true;
			}
			else {
				nameError = false;
				char const * lFilterPatterns[1] = { "*.animation" };
				const char* path = tinyfd_saveFileDialog("Save Animation...", (animName + ".animation").c_str(), 1, lFilterPatterns, NULL);
				if (path != NULL) {
					fs::path savedPath(path);
					savedPath.replace_extension(".animation");
					Data data;
					data.AddInt("Frames", animation->Size());
					data.AddFloat("FPS", animation->fps);
					data.AddBool("Loop", animation->loop);
					for (int i = 0; i < animation->Size(); i++) {
						string spriteNum = "Sprite" + to_string(i);
						data.CreateSection(spriteNum);
						data.AddString("SpritePath", animation->frames[i]->GetPath());
						data.AddString("SpriteName", animation->frames[i]->GetName());
						data.CloseSection();
					}
					data.SaveAsBinary(savedPath.string());
					ResourceAnimation* animationResource = new ResourceAnimation(savedPath.string(), animName);
					engine->resourcesModule->AddResourceAnimation(animationResource);
				}
			}
		}

		if (!isPlaying) {
			if (ImGui::Button("Play")) {
				isPlaying = true;
				isPaused = false;
			}
		}
		else {
			if (ImGui::Button("Pause")) {
				isPlaying = false;
				isPaused = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			if (isPlaying) {
				isPlaying = false;
				isPaused = true;
				animation->Reset();
				currentFrameIndex = animation->GetFrameIndex() + 1;
			}
		}
		ImGui::SameLine();
		if (ImGui::SliderInt("##Frames", &currentFrameIndex, 1, animation->Size())) {
			if (!isPlaying) {
				animation->SetInitialFrame(currentFrameIndex - 1);
			}
		}
		if (isPlaying) {
			ImGui::Image(*animation->GetCurrentFrame()->GetSprite(), sf::Color::White, sf::Color::Transparent);
			currentFrameIndex = animation->GetFrameIndex() + 1;
		}
		else if (isPaused) {
			ImGui::Image(*animation->GetActualFrame()->GetSprite(), sf::Color::White, sf::Color::Transparent);
		}
	
	}

	ImGui::End();

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::animationResource)) {
		ResourceSprite* sprite = (ResourceSprite*)engine->resourcesModule->DrawResourcesWindow(Resource::spriteResource);
		if (sprite != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, false);
			AddSpriteToAnimation(sprite);
		}
	}
}

void PanelAnimator::AddSpriteToAnimation(ResourceSprite* resource)
{
	animation->PushBack(resource);
	animation->SetInitialFrame(animation->Size() - 1);
	currentFrameIndex = animation->GetFrameIndex() + 1;
}
