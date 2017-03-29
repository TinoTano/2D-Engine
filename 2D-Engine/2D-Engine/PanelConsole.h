#pragma once

#include "Panel.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	virtual ~PanelConsole();

	void DrawPanel();
	void Clear();
	void AddLog(const char* fmt, ...) IM_PRINTFARGS(2);

private:
	ImGuiTextBuffer	Buf;
	ImGuiTextFilter Filter;
	ImVector<int> LineOffsets;
	bool ScrollToBottom;
};

