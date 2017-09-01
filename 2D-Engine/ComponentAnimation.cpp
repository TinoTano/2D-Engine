#include "ComponentAnimation.h"
#include "GameObject.h"

ComponentAnimation::ComponentAnimation(GameObject* attachedObject)
{
	gameObject = attachedObject;
	spriteRenderer = (ComponentSpriteRenderer*)gameObject->GetComponent(Component::SpriteRenderer);
	type = Animaton;
	name = "Animation";
}

ComponentAnimation::~ComponentAnimation()
{
	delete currentAnimation;
	delete texture;
}

void ComponentAnimation::OnEnable()
{
}

void ComponentAnimation::OnDisable()
{
}

void ComponentAnimation::Save(Data & data) const
{
}

void ComponentAnimation::Load(Data & data)
{
}

void ComponentAnimation::UpdateAnimation()
{
	if (isPlaying) {
		if (currentAnimation != nullptr) {
			spriteRenderer->ChangeSprite(currentAnimation->GetAnimation()->GetCurrentFrame()->GetSprite(), currentAnimation->GetAnimation()->GetCurrentFrame()->GetPath());
		}
	}
}

void ComponentAnimation::SetAnimation(ResourceAnimation* newAnimation)
{
	if (currentAnimation != nullptr) {
		Stop();
	}
	currentAnimation = newAnimation;
	spriteRenderer->ChangeSprite(currentAnimation->GetAnimation()->GetCurrentFrame()->GetSprite(), currentAnimation->GetAnimation()->GetCurrentFrame()->GetPath());
}

void ComponentAnimation::AddAnimation(ResourceAnimation * animation, int index)
{
	if (index == -1) {
		animationsList.push_back(animation);
	}
	else {
		if (index < animationsList.size() && index >= 0) {
			animationsList[index] = animation;
		}
		else {
			LOG_ERROR("Tying to add animation %s to invalid index!", animation->GetName().c_str());
		}
	}
}

void ComponentAnimation::RemoveAnimation(uint index)
{
	if (index < animationsList.size()) {
		animationsList.erase(animationsList.begin() + index);
	}
}

void ComponentAnimation::SetFPS(float time)
{
	currentAnimation->GetAnimation()->fps = time;
}

void ComponentAnimation::Play()
{
	isPlaying = true;
	playingAnimationName = currentAnimation->GetName();
}

void ComponentAnimation::Pause()
{
	isPlaying = false;
}

void ComponentAnimation::Stop()
{
	isPlaying = false;
	currentAnimation->GetAnimation()->Reset();
}

void ComponentAnimation::SetIsLoop(bool loop)
{
	currentAnimation->GetAnimation()->loop = loop;
}

void ComponentAnimation::SetColor(const sf::Color & color)
{
	//spriteRenderer->gameObject->gameObjectSprite->setColor(color);
}

ResourceAnimation * ComponentAnimation::GetAnimation() const
{
	return currentAnimation;
}

ResourceAnimation * ComponentAnimation::GetAnimation(string animName) const
{
	for (int i = 0; i < animationsList.size(); i++) {
		if (animationsList[i]->GetName() == animName) {
			return animationsList[i];
		}
	}
	return nullptr;
}

sf::FloatRect ComponentAnimation::GetLocalBounds() const
{
	return sf::FloatRect();
}

sf::FloatRect ComponentAnimation::GetGlobalBounds() const
{
	return sf::FloatRect();
}

bool ComponentAnimation::IsLoop() const
{
	return currentAnimation->GetAnimation()->loop;
}

bool ComponentAnimation::IsPlaying() const
{
	return isPlaying;
}

bool ComponentAnimation::IsFinish()
{
	return currentAnimation->GetAnimation()->Finished();
}

void ComponentAnimation::SetFrame(ResourceSprite * frame)
{
	currentAnimation->GetAnimation()->PushBack(frame);
}

void ComponentAnimation::RemoveFrame(ResourceSprite * frame)
{
	currentAnimation->GetAnimation()->RemoveFrame(frame);
}

vector<ResourceAnimation*> ComponentAnimation::GetAnimationList() const
{
	return animationsList;
}

string ComponentAnimation::GetPlayingAnimationName() const
{
	return playingAnimationName;
}

