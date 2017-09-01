#pragma once
#include "Component.h"
#include "ResourceParticleEffect.h"

class ComponentParticle : public Component
{
public:
	ComponentParticle(GameObject* attachedObject);
	virtual ~ComponentParticle();

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

	void Play();
	void Stop();
	void Pause();

	void AddEffect(ResourceParticleEffect*, int index);
	void RemoveEffect(int index);
	vector<ResourceParticleEffect*> GetParticleFXList() const;

private:
	vector<ResourceParticleEffect*> particleFXList;
};

