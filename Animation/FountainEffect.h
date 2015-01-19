#pragma once
#include "ParticleSystem.h"

class FountainEffect : public ParticleSystem
{
public:
	FountainEffect(void);
	FountainEffect(int numParticles);
	~FountainEffect(void);

	bool Initialise();
	void Update(float timeDelta);
	void InitParticle(Particle & particle);
	void UpdateParticle(float timeDelta, Particle & particle);
	float RandomClamped(float min, float max);

	int numParticles;
	glm::vec3 diffuse;
	glm::vec3 position;
};

