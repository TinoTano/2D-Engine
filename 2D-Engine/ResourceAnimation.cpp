#include "ResourceAnimation.h"
#include "Data.h"

ResourceAnimation::ResourceAnimation(string animPath, string name)
{
	animation = new Animation();
	Data data;
	data.LoadBinary(animPath);
	int size = data.GetInt("Frames");
	float fps = data.GetFloat("FPS");
	bool isLoop = data.GetBool("Loop");
	for (int i = 0; i < size; i++) {
		string spriteNum = "Sprite" + to_string(i);
		data.EnterSection(spriteNum);
		string spritePath = data.GetString("SpritePath");
		string spriteName = data.GetString("SpriteName");
		data.LeaveSection();
		ResourceSprite* sprite = new ResourceSprite(spritePath, spriteName);
		animation->PushBack(sprite);
	}
	animation->fps = fps;
	animation->loop = isLoop;
	this->name = name;
	resourceType = animationResource;
	path = animPath;
	uid = ++uid;
}

ResourceAnimation::~ResourceAnimation()
{
	delete animation;
}

Animation * ResourceAnimation::GetAnimation() const
{
	return animation;
}
