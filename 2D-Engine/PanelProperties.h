#pragma once

#include "Panel.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "ComponentScript.h"
#include "ComponentAnimation.h"
#include "ComponentAudio.h"
#include <afx.h>

class PanelProperties : public Panel
{
public:
	PanelProperties();
	virtual ~PanelProperties();

	void DrawPanel();
	void DrawComponent(Component* component);
	void DrawTransformPanel(ComponentTransform* transform);
	void DrawSpriteRendererPanel(ComponentSpriteRenderer* spriteRenderer);
	void DrawAnimatonPanel(ComponentAnimation* animator);
	void DrawAudioPanel(ComponentAudio* audio);
	void DrawScriptPanel(ComponentScript* script);
	bool ScriptHasChanged(string path);
	void UpdateScript(ComponentScript * script);

private:
	map<string, CTime> scriptsChangesData;
	int animPos = 0;
	int soundPos = 0;
	int musicPos = 0;
};

