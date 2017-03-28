#pragma once

#include "Panel.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"

class PanelDetails : public Panel
{
public:
	PanelDetails();
	virtual ~PanelDetails();

	void DrawPanel();
	void DrawComponent(Component* component);
	void DrawTransformPanel(ComponentTransform* transform);

};

