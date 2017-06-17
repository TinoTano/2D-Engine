#pragma once
#include "Component.h"
#include "ComponentSpriteRenderer.h"
#include "ResourceAnimation.h"

class ComponentAnimation : public Component
{
public:
	ComponentAnimation(GameObject* attachedObject);
	virtual ~ComponentAnimation();

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

	void UpdateAnimation();
	void SetAnimation(ResourceAnimation* animation);
	void AddAnimation(ResourceAnimation* animation, int pos);
	void RemoveAnimation(uint index);
	void SetFPS(float time);
	void Play();
	void Pause();
	void Stop();
	void SetIsLoop(bool loop);
	void SetColor(const sf::Color& color);
	ResourceAnimation* GetAnimation() const;
	ResourceAnimation* GetAnimation(string animName) const;
	sf::FloatRect GetLocalBounds() const;
	sf::FloatRect GetGlobalBounds() const;
	bool IsLoop() const;
	bool IsPlaying() const;
	bool IsFinish();
	void SetFrame(ResourceSprite* frame);
	void RemoveFrame(ResourceSprite* frame);

public:
	ResourceAnimation* currentAnimation = nullptr;
	vector<ResourceAnimation*> animationsList;
	ComponentSpriteRenderer* spriteRenderer = nullptr;
	bool isPlaying = false;
	const sf::Texture* texture = nullptr;
	string playingAnimationName;
};

