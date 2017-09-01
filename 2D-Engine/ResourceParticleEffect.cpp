#include "ResourceParticleEffect.h"
#include "Data.h"
#include "ResourceAnimation.h"

ResourceParticleEffect::ResourceParticleEffect(string pfxPath, string name)
{
	Data data;
	emitter = new ParticleEmitter();
	emitter->particle = new Particle();
	if (data.LoadBinary(pfxPath)) {
		if (data.EnterSection("Emitter")) {
			emitter->emitterPosition = data.GetVector2Float("Position");
			emitter->emitterRotation = data.GetFloat("Rotation");
			emitter->spawnLoop = data.GetBool("SpawnLoop");
			emitter->duration = data.GetFloat("Duration");
			emitter->startDelay = data.GetFloat("StartDelay");
			emitter->particleCount = data.GetInt("ParticleCount");
			emitter->spawnRate = data.GetFloat("SpawnRate");
			emitter->emitterSize = data.GetVector2Float("Size");
			emitter->emitterType = (ParticleEmitter::EmitterType)data.GetInt("Type");
			data.LeaveSection();

			if (data.EnterSection("Particle")) {
				emitter->particle->lifeTime = data.GetFloat("LifeTime");
				string spritePath = data.GetString("SpritePath");
				string spriteName = data.GetString("SpriteName");
				ResourceSprite* sprite = new ResourceSprite(spritePath, spriteName);
				emitter->particle->sprite = sprite;
				string animationPath = data.GetString("AnimationPath");
				string animationName = data.GetString("AnimationName");
				if (animationPath != "NULL" && animationName != "NULL") {
					ResourceAnimation* animation = new ResourceAnimation(animationPath, animationName);
					emitter->particle->animation = animation;
				}
				emitter->particle->particleRotation = data.GetFloat("Rotation");
				emitter->particle->scale = data.GetVector2Float("Scale");
				emitter->particle->speed = data.GetFloat("Speed");
				if (data.EnterSection("Color")) {
					int r = data.GetInt("r");
					int g = data.GetInt("g");
					int b = data.GetInt("b");
					int a = data.GetInt("a");
					emitter->particle->particleColor = sf::Color(r, g, b, a);
				}
				
				this->name = name;
				resourceType = particleFXResource;
				path = pfxPath;
				uid = ++uid;
			}
			else {
				LOG_ERROR("Particle data in %s is not found. Create a new particle effect.", name.c_str());
			}
		}
		else {
			LOG_ERROR("Emitter data in %s is not found. Create a new particle effect.", name.c_str());
		}
	}
	else {
		LOG_ERROR("Can not load particle effect %s. Is the path correct?", name.c_str());
	}
}

ResourceParticleEffect::~ResourceParticleEffect()
{
	delete emitter->particle;
	delete emitter;
}

ParticleEmitter * ResourceParticleEffect::GetEmitter() const
{
	return emitter;
}

Particle * ResourceParticleEffect::GetParticle() const
{
	return emitter->particle;;
}
