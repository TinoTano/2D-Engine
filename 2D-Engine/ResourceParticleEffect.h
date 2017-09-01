#pragma once
#include "Resource.h"
#include "ParticleSystem.h"

class ResourceParticleEffect : public Resource
{
public:
	ResourceParticleEffect(string animPath, string name);
	virtual ~ResourceParticleEffect();

	ParticleEmitter* GetEmitter() const;
	Particle* GetParticle() const;

private:
	ParticleEmitter* emitter;
};

