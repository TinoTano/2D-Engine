#pragma once
#include "Panel.h"
#include "ResourceAnimation.h"
#include "ResourceSprite.h"
#include "ParticleSystem.h"

class PanelParticleEditor : public Panel
{
public:
	PanelParticleEditor();
	virtual ~PanelParticleEditor();

	void DrawPanel();

private:
	ParticleEmitter* emitter = nullptr;
	bool nameError = false;
	sf::RenderTexture particleEffectWindow;
};

