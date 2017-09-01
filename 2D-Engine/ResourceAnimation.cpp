#include "ResourceAnimation.h"
#include "Data.h"

ResourceAnimation::ResourceAnimation(string animPath, string name)
{
	Data data;
	if (data.LoadBinary(animPath)) {
		animation = new Animation();
		int size = data.GetInt("Frames");
		float fps = data.GetFloat("FPS");
		bool isLoop = data.GetBool("Loop");
		for (int i = 0; i < size; i++) {
			string spriteNum = "Sprite" + to_string(i);
			if (data.EnterSection(spriteNum)) {
				string spritePath = data.GetString("SpritePath");
				string spriteName = data.GetString("SpriteName");
				data.LeaveSection();
				ResourceSprite* sprite = new ResourceSprite(spritePath, spriteName);
				animation->PushBack(sprite);
			}
			else {
				LOG_ERROR("Animation %s Sprite index %d data is not found.", name.c_str(), i);
			}
		}
		animation->fps = fps;
		animation->loop = isLoop;
		this->name = name;
		resourceType = animationResource;
		path = animPath;
		uid = ++uid;
	}
	else {
		LOG_ERROR("Can not load animation %s. Is the path correct?", name.c_str());
	}
}

ResourceAnimation::~ResourceAnimation()
{
	delete animation;
}

Animation * ResourceAnimation::GetAnimation() const
{
	return animation;
}
