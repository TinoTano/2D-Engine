#pragma once

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include "ResourceSprite.h"

typedef unsigned int uint;

class Animation
{
public:
	bool loop = true;
	float fps = 1.0f;
	std::vector<ResourceSprite*> frames;

private:
	float currentFrame = 0;
	int lastFrame = 0;
	int loops = 0;
	int lastLoop = 0;
	string animName;

public:

	void PushBack(ResourceSprite* sprite)
	{
		frames.push_back(sprite);
	}

	void RemoveFrame(const ResourceSprite* sprite)
	{
		for (int i = 0; i < frames.size(); i++) {
			if (frames[i]->uid == sprite->uid) {
				frames.erase(frames.begin() + i);
			}
		}
	}

	void RemoveFrame(const int index)
	{
		frames.erase(frames.begin() + index);
	}

	// return currentFrame+speed frame Rect
	ResourceSprite* GetCurrentFrame()
	{
		currentFrame += fps;
		if (currentFrame >= frames.size())
		{
			currentFrame = (loop) ? 0.0f : frames.size() - 1;
			loops++;
		}

		return frames[(int)currentFrame];
	}

	// return current_current frame Rect
	ResourceSprite* GetActualFrame() {

		return frames[(int)currentFrame];
	}

	float GetFrameIndex()const {
		return currentFrame;
	}

	void SetInitialFrame(uint frame) {
		currentFrame = (float)frame;
	}

	bool IsFrame(uint frame_index) const {
		return (currentFrame == frame_index);
	}

	void AnimForward() {
		currentFrame += fps;
		if (currentFrame >= lastFrame) currentFrame = 0;
	}

	void AnimBack() {
		currentFrame -= fps;
		if (currentFrame < 0)currentFrame = (float)lastFrame - 1;
	}

	bool Finished()
	{
		if (lastLoop != loops)
		{
			lastLoop++;
			return true;
		}
		return false;
	}

	void Reset()
	{
		currentFrame = 0;
		loops = 0;
	}

	int Size() const {
		return frames.size();
	}
};