#include "PanelConsole.h"



PanelConsole::PanelConsole()
{
	active = true;
	panelName = "Console";
	UpdateLabels();
}


PanelConsole::~PanelConsole()
{
}

void PanelConsole::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str(), false, false, ImGuiWindowFlags_HorizontalScrollbar)) {
		if (ImGui::Button(logsLabel.c_str())) {
			showLogs = !showLogs;
		}
		ImGui::SameLine();
		if (ImGui::Button(warningsLabel.c_str())) {
			showWarnings = !showWarnings;
		}
		ImGui::SameLine();
		if (ImGui::Button(errorsLabel.c_str())) {
			showErrors = !showErrors;
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			Clear();
		}

		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

		for (int i = 0; i < messageList.size(); i++) {
			if (showErrors && messageList[i].find("Error") != string::npos) {
				ImGui::TextColored(errorTextColor, "%s", messageList[i].c_str());
			}
			else if (showWarnings && messageList[i].find("Warning") != string::npos) {
				ImGui::TextColored(warningTextColor, "%s", messageList[i].c_str());
			}
			else if (showLogs && messageList[i].find("Log") != string::npos){
				ImGui::Text("%s", messageList[i].c_str());
			}
			else {
				ImGui::Text("");
			}
		}

		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	ImGui::EndDock();
}

void PanelConsole::Clear()
{
	messageList.clear();
	logs = 0;
	warnings = 0;
	errors = 0;
	UpdateLabels();
}

void PanelConsole::UpdateLabels()
{
	errorsLabel = "Errors (" + to_string(errors) + ")";
	warningsLabel = "Warnings (" + to_string(warnings) + ")";
	logsLabel = "Logs (" + to_string(logs) + ")";
}

void PanelConsole::AddLog(string log, bool isError, bool isWarning)
{
	string logPrefix;

	if (isError) {
		logPrefix += " [Error]  ";
		if (errors < 99) {
			errors++;
		}
		else {
			messageList.erase(messageList.begin());
		}
	}
	else if (isWarning) {
		logPrefix += " [Warning]  ";
		if (warnings < 99) {
			warnings++;
		}
		else {
			messageList.erase(messageList.begin());
		}
	}
	else {
		logPrefix += " [Log]  ";
		if (logs < 99) {
			logs++;
		}
		else {
			messageList.erase(messageList.begin());
		}
	}
	
	logPrefix += log;
	messageList.push_back(logPrefix);
	messageList.push_back("");
	ScrollToBottom = true;
	UpdateLabels();
}
