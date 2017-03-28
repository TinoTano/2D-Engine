#pragma once

#include <string>
#include "imgui-1.49\imgui.h"
#include "imgui-1.49\imgui-SFML.h"

using namespace std;

class Panel
{
public:
	Panel();
	virtual ~Panel();

	void SetActive(bool active);
	bool IsActive() const;
	void SetPos(ImVec2 pos);
	ImVec2 GetPos() const;
	virtual void DrawPanel() = 0;
	virtual void Resize(float w, float h) = 0;

public:
	bool active;
	string panelName;
	ImVec2 panelPosition;
	float panelWidth;
	float panelHeight;
};

