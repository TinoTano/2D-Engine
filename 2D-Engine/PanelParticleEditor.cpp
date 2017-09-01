#include "PanelParticleEditor.h"
#include "Engine.h"
#include "ModuleResources.h"
#include "tinyfiledialogs.h"
#include "Data.h"
#include "ModuleInput.h"
#include <experimental\filesystem>
namespace fs = experimental::filesystem;

PanelParticleEditor::PanelParticleEditor()
{
	active = false;
	panelName = "Particle Editor";
	emitter = new ParticleEmitter();
	emitter->particle = new Particle();
	nameError = false;
	particleEffectWindow.create(450, 450);
	particleEffectWindow.setSmooth(true);
}

PanelParticleEditor::~PanelParticleEditor()
{
	delete emitter->particle;
	delete emitter;
}

void PanelParticleEditor::DrawPanel()
{
	ImGui::SetNextWindowSize({ 960,540 });
	ImGui::Begin(panelName.c_str(), &active,
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders, emitter->isPlaying);

	ImGui::Columns(2);

	ImGui::Text("Effect Name");
	ImGui::SameLine(100);
	static char text[20];
	ImGui::InputText("##Name", text, 20);
	if (nameError) {
		ImGui::SameLine();
		ImGui::TextColored({ 1,0,0,1 }, "Invalid name!");
	}
	ImGui::Spacing();
	ImGui::Text("Emitter :");
	ImGui::Spacing();
	ImGui::Text("Position");
	ImGui::SameLine(100);
	ImGui::DragFloat2("##EmitterPosition", (float*)&emitter->emitterPosition, 0.25f);
	ImGui::Text("Rotation");
	ImGui::SameLine(100);
	ImGui::DragFloat("##EmitterRotation", &emitter->emitterRotation, 0.25f);
	ImGui::Text("Loop");
	ImGui::SameLine(100);
	ImGui::Checkbox("##SpawnLoop", &emitter->spawnLoop);
	ImGui::Text("Duration");
	ImGui::SameLine(100);
	ImGui::DragFloat("##Duration", &emitter->duration, 0.25f, 0, FLT_MAX);
	ImGui::Text("Start Delay");
	ImGui::SameLine(100);
	ImGui::DragFloat("##StartDelay", &emitter->startDelay, 0.25f, 0, FLT_MAX);
	ImGui::Text("Particle Count");
	ImGui::SameLine(100);
	ImGui::DragInt("##ParticleCount", &emitter->particleCount, 1, 0, MAX_PARTICLES);
	ImGui::Text("Spawn Rate");
	ImGui::SameLine(100);
	if (emitter->particleCount < emitter->spawnRate) emitter->spawnRate = emitter->particleCount;
	ImGui::DragInt("##SpawnRate", &emitter->spawnRate, 1, 0, emitter->particleCount);
	ImGui::Text("Emitter Size");
	ImGui::SameLine(100);
	ImGui::DragFloat2("##EmitterSize", (float*)&emitter->emitterSize, 0.25f, 0, FLT_MAX);
	ImGui::Text("Emitter Type");
	ImGui::SameLine(100);
	ImGui::Combo("##EmitterType", (int*)&emitter->emitterType, "Circle\0Edge\0Cone\0");
	ImGui::Spacing();

	ImGui::Text("Particle :");
	ImGui::Spacing();
	ImGui::Text("Life Time");
	ImGui::SameLine(100);
	ImGui::DragFloat("##LifeTime", &emitter->particle->lifeTime, 0.25f, 0, FLT_MAX);
	ImGui::Text("Sprite");
	ImGui::SameLine(100);
	ImGui::Text(emitter->particle->sprite->GetName().c_str());
	ImGui::SameLine();
	if (ImGui::Button("Add##Sprite")) {
		engine->resourcesModule->SetResourcesWindowOpen(Resource::spriteResource, true);
		engine->resourcesModule->isOpenFromPanel = "Particle Editor";
	}
	ImGui::Text("Animation");
	ImGui::SameLine(100);
	if (emitter->particle->animation != nullptr) {
		ImGui::Text(emitter->particle->animation->GetName().c_str());
		ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			emitter->particle->animation = nullptr;
		}
	}
	else {
		ImGui::Text("No Animation");
		ImGui::SameLine();
		if (ImGui::Button("Add##Animation")) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, true);
			engine->resourcesModule->isOpenFromPanel = "Particle Editor";
		}
	}
	ImGui::Text("Rotation");
	ImGui::SameLine(100);
	ImGui::DragFloat("##ParticleRotation", &emitter->particle->particleRotation, 0.25f, 0, FLT_MAX);
	ImGui::Text("Scale");
	ImGui::SameLine(100);
	ImGui::DragFloat2("##ParticleScale", (float*)&emitter->particle->scale, 0.01f, 0, FLT_MAX);
	ImGui::Text("Speed");
	ImGui::SameLine(100);
	ImGui::DragFloat("##ParticleSpeed", &emitter->particle->speed, 0.25f, 0, FLT_MAX);
	ImGui::Text("Color");
	ImGui::SameLine(100);
	int color[4] = { emitter->particle->particleColor.r, emitter->particle->particleColor.g, emitter->particle->particleColor.b, emitter->particle->particleColor.a };
	if (ImGui::DragInt4("##ParticleColor", color, 0.25f, 0, 255)) {
		emitter->particle->particleColor = sf::Color(color[0], color[1], color[2], color[3]);
	}
	ImGui::Spacing();
	if (ImGui::Button("Save Particle FX")) {
		string fxName(text);
		if (fxName.empty() || fxName[0] == ' ') {
			nameError = true;
		}
		else {
			nameError = false;
			char const * lFilterPatterns[1] = { "*.particleFX" };
			const char* path = tinyfd_saveFileDialog("Save Particle FX...", (fxName + ".particleFX").c_str(), 1, lFilterPatterns, NULL);
			if (path != NULL) {
				fs::path savedPath(path);
				savedPath.replace_extension(".particleFX");
				Data data;
				data.CreateSection("Emitter");
				data.AddVector2Float("Position", emitter->emitterPosition);
				data.AddFloat("Rotation", emitter->emitterRotation);
				data.AddBool("SpawnLoop", emitter->spawnLoop);
				data.AddFloat("Duration", emitter->duration);
				data.AddFloat("StartDelay", emitter->startDelay);
				data.AddInt("ParticleCount", emitter->particleCount);
				data.AddInt("SpawnRate", emitter->spawnRate);
				data.AddVector2Float("Size", emitter->emitterSize);
				data.AddInt("Type", emitter->emitterType);

				data.CreateSection("Particle");
				data.AddFloat("LifeTime", emitter->particle->lifeTime);
				data.AddString("SpritePath", emitter->particle->sprite->GetPath());
				data.AddString("SpriteName", emitter->particle->sprite->GetName());
				if (emitter->particle->animation != nullptr) {
					data.AddString("AnimationPath", emitter->particle->animation->GetPath());
					data.AddString("AnimationName", emitter->particle->animation->GetName());
				}
				else {
					data.AddString("AnimationPath", "NULL");
					data.AddString("AnimationName", "NULL");
				}
				data.AddFloat("Rotation", emitter->particle->initialRotation);
				data.AddVector2Float("Scale", emitter->particle->scale);
				data.AddFloat("Speed", emitter->particle->speed);
				data.CreateSection("Color");
				data.AddInt("r", emitter->particle->initialColor.r);
				data.AddInt("g", emitter->particle->initialColor.g);
				data.AddInt("b", emitter->particle->initialColor.b);
				data.AddInt("a", emitter->particle->initialColor.a);
				data.CloseSection();
				data.CloseSection();
				data.CloseSection();
				data.SaveAsBinary(savedPath.string());
				data.SaveAsXML(savedPath.string());
				ResourceParticleEffect* pfxResource = new ResourceParticleEffect(savedPath.string(), fxName);
				engine->resourcesModule->AddResourceParticleFX(pfxResource);
			}
		}
	}

	ImGui::NextColumn();

	ImGui::BeginChild("Emitter");
	particleEffectWindow.clear(sf::Color(60, 60, 60, 255));

	switch (emitter->emitterType) {
	case ParticleEmitter::CircleEmitter:
	{
		sf::CircleShape circle(emitter->emitterSize.x);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineThickness(1);
		circle.setOutlineColor(sf::Color::White);
		circle.setOrigin(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2);
		circle.setPosition(particleEffectWindow.getSize().x / 2 + emitter->emitterPosition.x, particleEffectWindow.getSize().y / 2 + emitter->emitterPosition.y);
		particleEffectWindow.draw(circle);
		emitter->spawnPosition = circle.getPosition();
	}
	break;
	case ParticleEmitter::EdgeEmitter:
	{
		sf::RectangleShape line(sf::Vector2f(emitter->emitterSize.x * 2, 1));
		line.setOrigin(line.getLocalBounds().width / 2, line.getLocalBounds().height / 2);
		line.setPosition(particleEffectWindow.getSize().x / 2 + emitter->emitterPosition.x, particleEffectWindow.getSize().y / 2 + emitter->emitterPosition.y);
		particleEffectWindow.draw(line);
		emitter->spawnPosition = line.getPosition();
	}
	break;
	case ParticleEmitter::ConeEmitter:
	{
		sf::VertexArray triangle(sf::LineStrip, 4);
		triangle[0].position = sf::Vector2f(particleEffectWindow.getSize().x / 2 + emitter->emitterPosition.x, particleEffectWindow.getSize().y / 2 + emitter->emitterPosition.y);
		triangle[1].position = triangle[0].position - sf::Vector2f(emitter->emitterSize.x, -emitter->emitterSize.y * 2);
		triangle[2].position = triangle[0].position + sf::Vector2f(emitter->emitterSize.x, emitter->emitterSize.y * 2);
		triangle[3].position = triangle[0].position;
		triangle[0].color = sf::Color::White;
		triangle[1].color = sf::Color::White;
		triangle[2].color = sf::Color::White;
		triangle[3].color = sf::Color::White;
		emitter->spawnPosition = triangle[0].position;
		emitter->conePoints[0] = triangle[0].position - emitter->spawnPosition;
		emitter->conePoints[1] = triangle[1].position - emitter->spawnPosition;
		emitter->conePoints[2] = triangle[2].position - emitter->spawnPosition;
		particleEffectWindow.draw(triangle);
	}
	break;
	}

	if (!emitter->isPlaying) {
		if (ImGui::Button("Play")) {
			emitter->StartEmitter();
		}
	}
	else {
		if (ImGui::Button("Pause")) {
			emitter->isPlaying = false;
			emitter->isPaused = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		if (emitter->isPlaying) {
			emitter->StopEmitter();
		}
	}
	ImGui::SameLine();
	ImGui::Text(" Time:");
	ImGui::SameLine();
	if (emitter->timerRunning) {
		ImGui::Text("%.f", emitter->timer.getElapsedTime().asSeconds());
	}
	else {
		ImGui::Text("0");
	}

	ImGui::SameLine();
	ImGui::Text(" Particles count:  %d", emitter->particlesAliveCount);

	if (emitter->readyToPlay) {
		emitter->UpdateParticles(particleEffectWindow);
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Image((void*)particleEffectWindow.getTexture().getNativeHandle(), ImVec2(450,450), ImVec2(0, 1), ImVec2(1, 0), sf::Color::White, sf::Color::White);
	if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax())) {
		if (engine->inputModule->IsMouseWheelScrollingDown()) {
			sf::View view = particleEffectWindow.getView();
			view.zoom(1.1f);
			particleEffectWindow.setView(view);
		}
		else if (engine->inputModule->IsMouseWheelScrollingUp()) {
			sf::View view = particleEffectWindow.getView();
			view.zoom(0.9f);
			particleEffectWindow.setView(view);
		}
	}
	ImGui::EndChild();
	
	ImGui::End();

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::spriteResource) && engine->resourcesModule->isOpenFromPanel == "Particle Editor") {
		ResourceSprite* sprite = (ResourceSprite*)engine->resourcesModule->DrawResourcesWindow(Resource::spriteResource);
		if (sprite != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::spriteResource, false);
			emitter->particle->sprite = sprite;
		}
	}

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::animationResource) && engine->resourcesModule->isOpenFromPanel == "Particle Editor") {
		ResourceAnimation* animation = (ResourceAnimation*)engine->resourcesModule->DrawResourcesWindow(Resource::animationResource);
		if (animation != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, false);
			emitter->particle->animation = animation;
		}
	}
}

