#pragma once

#include "Panel.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "ComponentScript.h"
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
	void DrawScriptPanel(ComponentScript* script);
	bool ScriptHasChanged(string path);
	void UpdateScript(ComponentScript * script);

private:
	map<string, CTime> scriptsChangesData;
};

