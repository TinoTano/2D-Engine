#include "Panel.h"



Panel::Panel()
{
	active = true;
	panelName = "";
	dockPos = ImGuiDockSlot_None;
	newPanel = false;
}

Panel::~Panel()
{
}

void Panel::SetActive(bool active)
{
	this->active = active;
}

bool Panel::IsActive() const
{
	return active;
}

