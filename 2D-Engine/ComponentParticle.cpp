#include "ComponentParticle.h"


ComponentParticle::ComponentParticle(GameObject* attachedObject)
{
	gameObject = attachedObject;
	type = ParticleSystem;
	name = "Particle effect";
}

ComponentParticle::~ComponentParticle()
{
}

void ComponentParticle::OnEnable()
{
}

void ComponentParticle::OnDisable()
{
}

void ComponentParticle::Save(Data & data) const
{
}

void ComponentParticle::Load(Data & data)
{
}

void ComponentParticle::Play()
{
}

void ComponentParticle::Stop()
{
}

void ComponentParticle::Pause()
{
}

void ComponentParticle::AddEffect(ResourceParticleEffect* particleFX, int index)
{
	if (index == -1) {
		particleFXList.push_back(particleFX);
	}
	else {
		if (index < particleFXList.size() && index >= 0) {
			particleFXList[index] = particleFX;
		}
		else {
			LOG_ERROR("Tying to add particleFX %s to invalid index!", particleFX->GetName().c_str());
		}
	}
}

void ComponentParticle::RemoveEffect(int index)
{
	if (index < particleFXList.size()) {
		particleFXList.erase(particleFXList.begin() + index);
	}
}

vector<ResourceParticleEffect*> ComponentParticle::GetParticleFXList() const
{
	return particleFXList;
}
