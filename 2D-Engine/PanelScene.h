#pragma once

#include "Panel.h"

class PanelScene : public Panel
{
public:
	PanelScene();
	virtual ~PanelScene();
	
	void DrawPanel();

private:
	sf::Vector2f storedSceneSize;
};

