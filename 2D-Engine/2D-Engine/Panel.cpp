#include "Panel.h"



Panel::Panel()
{
	active = true;
	panelName = "";
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

