#pragma once

#include "Panel.h"
#include <vector>
#include <SFML\Graphics.hpp>

class PanelConsole : public Panel
{
public:
	PanelConsole();
	virtual ~PanelConsole();

	void DrawPanel();
	void Clear();
	void AddLog(string log, bool isError = false, bool isWarning = false);
	void UpdateLabels();

private:
	ImGuiTextBuffer	Buf;
	ImGuiTextFilter Filter;
	ImVector<int> LineOffsets;
	bool ScrollToBottom = false;
	int logs = 0;
	int warnings = 0;
	int errors = 0;
	string logsLabel;
	string warningsLabel;
	string errorsLabel;
	bool showLogs = true;
	bool showWarnings = true;
	bool showErrors = true;
	vector<string> messageList;
	ImVec4 logTextColor = { 1.00f, 1.00f ,1.00f ,1.00f };
	ImVec4 warningTextColor = { 1.00f, 1.00f, 0.00f ,1.00f };
	ImVec4 errorTextColor = { 1.00f, 0.00f, 0.00f ,1.00f };
};

