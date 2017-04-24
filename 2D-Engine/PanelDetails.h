#pragma once

#include "Panel.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"

class PanelDetails : public Panel
{
public:
	PanelDetails();
	virtual ~PanelDetails();

	void DrawPanel();
	void DrawComponent(Component* component);
	void DrawTransformPanel(ComponentTransform* transform);
	void DrawSpriteRendererPanel(ComponentSpriteRenderer* spriteRenderer);

private:
	bool flipX = false;
	bool flipY = false;

};

