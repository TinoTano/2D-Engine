#pragma once

#include <string>
#include "imgui-1.49\imgui.h"
#include "imgui-1.49\imgui-SFML.h"
#include "imgui-1.49\imguidock.h"

using namespace std;

class Panel
{
public:
	Panel();
	virtual ~Panel();

	void SetActive(bool active);
	bool IsActive() const;
	virtual void DrawPanel() = 0;

public:
	bool active;
	string panelName;
	ImGuiDockSlot dockPos;
	bool newPanel;
};

