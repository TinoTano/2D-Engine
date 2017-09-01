#pragma once
#include <SFML\System\Vector3.hpp>
#include <SFML\System\vector2.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Window.hpp>
#include "ResourceAnimation.h"
#include "ResourceSprite.h"

#define MAX_PARTICLES 1000

struct Particle {

	float lifeTime;
	ResourceSprite* sprite;
	ResourceAnimation* animation = nullptr;
	bool isAlive;
	sf::Vector2f particlePosition;
	float particleRotation;
	//Collider
	sf::Color particleColor;
	sf::Vector2f velocity;
	float speed;
	sf::Color initialColor;
	float initialRotation;
	sf::Clock lifeTimeTimer;
	vector<sf::Sprite*> animationSprites;
	int animationIndex;
	sf::Vector2f scale;

	Particle() {
		lifeTime = 5;
		sprite = new ResourceSprite("../Data/particle.png", "Basic Particle");
		particleColor = sf::Color::White;
		initialColor = particleColor;
		isAlive = false;
		particleRotation = 0;
		initialRotation = 0;
		speed = 0;
		animationIndex = 0;
		scale = { 1,1 };
	}
};

class ParticleEmitter {

public:
	enum EmitterType {
		CircleEmitter, EdgeEmitter, ConeEmitter
	};

	ParticleEmitter();

	sf::Vector2f emitterPosition;
	float emitterRotation = 0;
	bool spawnLoop = false;
	float duration = 10;
	float durationOffset = 0;
	float startDelay = 0;
	float startDelayOffset = 0;
	sf::Clock timer;
	list<Particle*> particlesList;
	list<Particle*> spawnedParticles;
 	int particleCount = 50;
	int spawnRate = 50;
	float spawnRateOffset = 0;
	sf::Vector2f emitterSize = { 50.f,50.f };
	EmitterType emitterType = CircleEmitter;
	Particle* particle = nullptr;
	int particlesAliveCount = 0;
	bool readyToPlay = false;
	sf::Vector2f spawnPosition;
	bool timerRunning = false;
	float spawnTime = 0;
	sf::Clock spawnerTimer;
	bool isPlaying = false;
	bool isPaused = true;
	sf::Vector2f conePoints[3];

	void StartEmitter();
	void UpdateParticles(sf::RenderTexture& target);
	void ResetParticle(Particle* particle);
	void ResetEmitter();
	void StopEmitter();
	void CleanEmitter();
	void SpawnParticle();
	void RandomParticlePosition(Particle* particle);
	void UpdateParticleStats(Particle* particle);

};