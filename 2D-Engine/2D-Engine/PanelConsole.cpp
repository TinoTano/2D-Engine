#include "PanelConsole.h"



PanelConsole::PanelConsole()
{
	active = true;
	panelName = "Console";
	dockPos = ImGuiDockSlot_Top;
	newPanel = true;
}


PanelConsole::~PanelConsole()
{
}

void PanelConsole::DrawPanel()
{
}
