#include "ParticleSystem.h"
#include <random>
#include <math.h>

ParticleEmitter::ParticleEmitter()
{

}

void ParticleEmitter::StartEmitter()
{
	readyToPlay = false;
	if (particlesList.size() < particleCount) {
		while (particlesList.size() < particleCount) {
			Particle* tempParticle = new Particle();
			particlesList.push_back(tempParticle);
		}
	}
	else if (particlesList.size() > particleCount) {
		particlesList.erase(particlesList.begin());
	}
	spawnTime = 1.f / spawnRate;
	readyToPlay = true;
	timerRunning = true;
	timer.restart();
	isPlaying = true;
	isPaused = false;
}

void ParticleEmitter::UpdateParticles(sf::RenderTexture& target)
{
	float time = timer.getElapsedTime().asSeconds();

	if (time > duration) {
		if (spawnLoop) {
			ResetEmitter();
		}
		else {
			StopEmitter();
		}
	}
	else {
		if (time > startDelay) {
			if (spawnerTimer.getElapsedTime().asSeconds() > spawnTime && spawnTime != 0) {
				SpawnParticle();
				spawnerTimer.restart();
			}
			list<Particle*>::iterator it = spawnedParticles.begin();
			while (it != spawnedParticles.end()) {
				float t = (*it)->lifeTimeTimer.getElapsedTime().asSeconds();
				if ((*it)->lifeTimeTimer.getElapsedTime().asSeconds() > (*it)->lifeTime) {
					ResetParticle(*it);
					spawnedParticles.erase(it++);
				}
				else {
					(*it)->particlePosition += (*it)->velocity;
					sf::Sprite sprite;
					if (!(*it)->animationSprites.empty()) {
						if ((*it)->animationIndex == (*it)->animationSprites.size() - 1) {
							(*it)->animationIndex = 0;
						}
						sprite = *(*it)->animationSprites[(*it)->animationIndex++];
					}
					else {
						sprite = *(*it)->sprite->GetSprite();
					}
					sprite.setPosition((*it)->particlePosition);
					target.draw(sprite);
					it++;
				}
			}
		}
	}
}

void ParticleEmitter::ResetParticle(Particle* particle)
{
	particle->particleColor = particle->initialColor;
	particle->particleRotation = particle->initialRotation;
	if (particle->animation != nullptr) {
		particle->animation->GetAnimation()->SetInitialFrame(0);
	}
	particle->isAlive = false;
	particlesAliveCount--;
	particlesList.push_back(particle);
}

void ParticleEmitter::ResetEmitter()
{
	list<Particle*>::iterator it = spawnedParticles.begin();
	while (it != spawnedParticles.end()) {
		ResetParticle(*it);
		spawnedParticles.erase(it++);
	}
	timer.restart();
}

void ParticleEmitter::StopEmitter()
{
	list<Particle*>::iterator it = spawnedParticles.begin();
	while (it != spawnedParticles.end()) {
		ResetParticle(*it);
		spawnedParticles.erase(it++);
	}
	timerRunning = false;
	readyToPlay = false;
	isPlaying = false;
	isPaused = true;
}

void ParticleEmitter::CleanEmitter()
{
	for (list<Particle*>::iterator it = particlesList.begin(); it != particlesList.end(); it++) {
		RELEASE(*it);
	}
	particlesList.clear();

	for (list<Particle*>::iterator it = spawnedParticles.begin(); it != spawnedParticles.end(); it++) {
		RELEASE(*it);
	}
	spawnedParticles.clear();
}

void ParticleEmitter::SpawnParticle()
{
	if (particlesAliveCount < particleCount) {
		particlesList.front()->lifeTimeTimer.restart();
		UpdateParticleStats(particlesList.front());
		RandomParticlePosition(particlesList.front());
		spawnedParticles.push_back(particlesList.front());
		particlesList.remove(particlesList.front());
		particlesAliveCount++;
	}
}

void ParticleEmitter::RandomParticlePosition(Particle * particle)
{
	sf::Vector2f randPos(0, 0);
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> disX(-emitterSize.x, emitterSize.x);
	std::uniform_real_distribution<> disY(-emitterSize.y, emitterSize.y);
	std::uniform_real_distribution<> randNegative(-1, 1);
	float r;
	switch (emitterType) {
	case EmitterType::CircleEmitter:
		randPos.x = disX(gen);
		randPos.y = randNegative(gen) * sqrt(emitterSize.x * emitterSize.x - randPos.x*randPos.x);
		r = sqrt(randPos.x * randPos.x + randPos.y*randPos.y);
		particle->velocity.x = (randPos.x / r) * particle->speed;
		particle->velocity.y = (randPos.y / r) * particle->speed;
		break;
	case EmitterType::ConeEmitter:
	{
		sf::Vector2f u = conePoints[1] - conePoints[0];
		sf::Vector2f v = conePoints[2] - conePoints[0];
		std::uniform_real_distribution<> x(0, 1);
		std::uniform_real_distribution<> y(0, 1);
		sf::Vector2f unit(x(gen), y(gen));
		if ((unit.x + unit.y) >= 1) {
			unit = sf::Vector2f(1 - unit.x, 1 - unit.y);
		}
		randPos = conePoints[0] + unit.x * u + unit.y * v;
		r = sqrt(randPos.x * randPos.x + randPos.y*randPos.y);
		particle->velocity.x = (randPos.x / r) * particle->speed;
		particle->velocity.y = (randPos.y / r) * particle->speed;
	}
		break;
	case EmitterType::EdgeEmitter:
		randPos.x = disX(gen);
		particle->velocity.x = 0;
		particle->velocity.y = particle->speed;
		break;
	}
	particle->particlePosition = spawnPosition + randPos;
}

void ParticleEmitter::UpdateParticleStats(Particle * particle)
{
	particle->lifeTime = this->particle->lifeTime;
	particle->sprite = this->particle->sprite;
	particle->sprite->GetSprite()->setColor(this->particle->particleColor);
	particle->sprite->GetSprite()->setRotation(this->particle->particleRotation);
	particle->sprite->GetSprite()->setScale(this->particle->scale);
	particle->animation = this->particle->animation;
	if (particle->animation != nullptr) {
		particle->animationSprites.clear();
		for (int i = 0; i < this->particle->animation->GetAnimation()->frames.size(); i++) {
			particle->animationSprites.push_back(this->particle->animation->GetAnimation()->frames[i]->GetSprite());
		}
		particle->animationIndex = 0;
	}
	particle->speed = this->particle->speed;
}

