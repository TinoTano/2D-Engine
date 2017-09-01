#pragma once
#include "Module.h"
#include "Animation.h"

class Emitter {

public:
	enum ParticleType {
		CircleParticle, SquareParticle, SpriteParticle
	};

	sf::Vector3f emitterPosition;
	float emitterRotation;
	bool spawnloop;
	float duration;
	float durationOffset;
	float startDelay;
	float startDelayOffset;
	sf::Clock timer;
	vector<Particle*> particlesList;
	int particleCount;
	float spawnRate;
	float spawnRateOffset;
	sf::Vector2f emitterSize;
};

class Particle {

public:
	float lifeTime;
	sf::Sprite sprite;
	Animation* anim;
	bool isAlive;
	sf::Vector3f particlePosition;
	float particlesRotation;
	//Collider
	sf::Color particleColor;
	sf::Vector2f velocity;
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	virtual ~ModuleParticles();

	bool Start();
	bool Update();
	bool CleanUp();

};

