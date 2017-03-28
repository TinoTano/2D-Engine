#include "Component.h"



Component::Component()
{
	type = Unknown;
	active = true;
}


Component::~Component()
{
}

void Component::OnEnable()
{
}

void Component::OnDisable()
{
}

void Component::SetActive(bool active)
{
	this->active = active;
	if (this->active) {
		OnEnable();
	}
	else {
		OnDisable();
	}
}

bool Component::IsActive() const
{
	return active;
}
