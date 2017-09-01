#include "ModuleResources.h"

ModuleResources::ModuleResources()
{
	moduleName = "Resources Manager";
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Awake()
{
	return true;
}

bool ModuleResources::CleanUp()
{
	return true;
}

vector<ResourceAnimation*> ModuleResources::GetGameAnimationsList()
{
	return gameAnimationsList;
}

vector<ResourceScript*> ModuleResources::GetGameScriptsList()
{
	return gameScriptsList;
}

vector<ResourceSprite*> ModuleResources::GetGameSpritesList()
{
	return gameSpritesList;
}

vector<ResourceSound*> ModuleResources::GetGameSoundsList()
{
	return gameSoundsList;
}

vector<ResourceMusic*> ModuleResources::GetGameMusicList()
{
	return gameMusicList;
}

vector<ResourceParticleEffect*> ModuleResources::GetGameParticleFXList()
{
	return gameParticleFXList;
}

void ModuleResources::AddResourceAnimation(ResourceAnimation * anim)
{
	gameAnimationsList.push_back(anim);
}

void ModuleResources::AddResourceScript(ResourceScript * script)
{
	gameScriptsList.push_back(script);
}

void ModuleResources::AddResourceSprite(ResourceSprite * sprite)
{
	gameSpritesList.push_back(sprite);
}

void ModuleResources::AddResourceSound(ResourceSound * sound)
{
	gameSoundsList.push_back(sound);
}

void ModuleResources::AddResourceMusic(ResourceMusic * music)
{
	gameMusicList.push_back(music);
}

void ModuleResources::AddResourceParticleFX(ResourceParticleEffect * particleFX)
{
	gameParticleFXList.push_back(particleFX);
}

void* ModuleResources::DrawResourcesWindow(Resource::ResourceType type)
{
	void* ret = nullptr;

	ImGui::SetNextWindowSize({ 500,700 });
	if (ImGui::Begin("Resources", &resourcesWindowIsOpen,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders)) 
	{
		switch (type) {
		case Resource::animationResource:
			for (vector<ResourceAnimation*>::iterator it = gameAnimationsList.begin(); it != gameAnimationsList.end(); it++) {
				if (ImGui::Selectable((*it)->GetName().c_str())) {
					ret = (*it);
					break;
				}
			}
			break;
		case Resource::spriteResource:
			for (vector<ResourceSprite*>::iterator it = gameSpritesList.begin(); it != gameSpritesList.end(); it++) {
				if (ImGui::Selectable((*it)->GetName().c_str())) {
					ret = (*it);
					break;
				}
			}
			break;
		case Resource::scriptResource:
			for (vector<ResourceScript*>::iterator it = gameScriptsList.begin(); it != gameScriptsList.end(); it++) {
				if (ImGui::Selectable((*it)->GetName().c_str())) {
					ret = (*it);
					break;
				}
			}
			break;
		case Resource::soundResource:
			for (vector<ResourceSound*>::iterator it = gameSoundsList.begin(); it != gameSoundsList.end(); it++) {
				if (ImGui::Selectable((*it)->GetName().c_str())) {
					ret = (*it);
					break;
				}
			}
			break;
		case Resource::musicResource:
			for (vector<ResourceMusic*>::iterator it = gameMusicList.begin(); it != gameMusicList.end(); it++) {
				if (ImGui::Selectable((*it)->GetName().c_str())) {
					ret = (*it);
					break;
				}
			}
			break;
		case Resource::particleFXResource:
			for (vector<ResourceParticleEffect*>::iterator it = gameParticleFXList.begin(); it != gameParticleFXList.end(); it++) {
				if (ImGui::Selectable((*it)->GetName().c_str())) {
					ret = (*it);
					break;
				}
			}
			break;
		}

		ImGui::End();
	}

	return ret;
}

bool ModuleResources::IsResourcesWindowOpen(Resource::ResourceType type)
{
	bool ret = false;

	if (resourcesWindowIsOpen)
	{
		switch (type) {
		case Resource::animationResource:
			ret = animationsWindowIsOpen;
			break;
		case Resource::spriteResource:
			ret = spritesWindowIsOpen;
			break;
		case Resource::scriptResource:
			ret = scriptsWindowIsOpen;
			break;
		case Resource::soundResource:
			ret = soundsWindowIsOpen;
			break;
		case Resource::musicResource:
			ret = musicsWindowIsOpen;
			break;
		case Resource::particleFXResource:
			ret = particleFXWindowIsOpen;
			break;
		}
	}

	return ret;
}

void ModuleResources::SetResourcesWindowOpen(Resource::ResourceType type, bool open)
{
	resourcesWindowIsOpen = open;
	animationsWindowIsOpen = spritesWindowIsOpen = scriptsWindowIsOpen = soundsWindowIsOpen = musicsWindowIsOpen = particleFXWindowIsOpen = false;
	switch (type) {
	case Resource::animationResource:
		animationsWindowIsOpen = open;
		break;
	case Resource::spriteResource:
		spritesWindowIsOpen = open;
		break;
	case Resource::scriptResource:
		scriptsWindowIsOpen = open;
		break;
	case Resource::soundResource:
		soundsWindowIsOpen = open;
		break;
	case Resource::musicResource:
		musicsWindowIsOpen = open;
		break;
	case Resource::particleFXResource:
		particleFXWindowIsOpen = open;
		break;
	}
}



