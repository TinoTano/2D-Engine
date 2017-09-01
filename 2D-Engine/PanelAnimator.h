#pragma once
#include "Panel.h"
#include "Animation.h"
#include "ResourceSprite.h"

class PanelAnimator : public Panel
{
public:
	PanelAnimator();
	virtual ~PanelAnimator();

	void DrawPanel();

	void AddSpriteToAnimation(ResourceSprite* sprite);

private:
	bool creatingAnimation;
	Animation* animation = nullptr;
	int currentFrameIndex;
	bool isPlaying;
	bool isPaused;
	bool nameError;
};

