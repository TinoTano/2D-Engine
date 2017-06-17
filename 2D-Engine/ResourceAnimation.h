#pragma once
#include "Resource.h"
#include "Animation.h"

class ResourceAnimation : public Resource
{
public:
	ResourceAnimation(string animPath, string name);
	virtual ~ResourceAnimation();

	Animation* GetAnimation() const;

private:
	Animation* animation = nullptr;

};

