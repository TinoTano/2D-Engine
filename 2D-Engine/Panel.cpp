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


