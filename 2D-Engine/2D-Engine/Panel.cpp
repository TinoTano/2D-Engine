#include "Panel.h"



Panel::Panel()
{
	active = true;
	panelName = "";
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

void Panel::SetPos(ImVec2 pos)
{
	panelPosition = pos;
}

ImVec2 Panel::GetPos() const
{
	return panelPosition;
}
