#include "FountainEffect.h"

FountainEffect::FountainEffect(void)
{
}

FountainEffect::FountainEffect(int numParticles)
{
	this->numParticles = numParticles;
	diffuse = glm::vec3(0,0,1);
}

FountainEffect::~FountainEffect(void)
{
}

bool FountainEffect::Initialise()
{
	for (int i = 0 ; i < numParticles ; i ++)
	{
		Particle p;
		InitParticle(p);
		particles.push_back(p);
	}
	return ParticleSystem::Initialise();
}

void FountainEffect::Update(float timeDelta)
{
	ParticleSystem::Update(timeDelta);
}

void FountainEffect::InitParticle(Particle & particle)
{
	float radius = 0.25f;
	particle.position = glm::vec3(0,0,0);

	particle.velocity = glm::vec3(RandomClamped(-radius, radius), RandomClamped(-1.0f,1.0f), RandomClamped(-radius, radius)) * 10.0f;
	particle.velocity.y = glm::abs<float>(particle.velocity.y);
	particle.diffuse.r = diffuse.r * RandomClamped(0, 1);
	particle.diffuse.g = diffuse.g * RandomClamped(0, 1);
	particle.diffuse.b = diffuse.b * RandomClamped(0, 1);
	particle.age = 0;
	particle.alive = true;
	particle.size = RandomClamped(10, 50);
	particle.lifetime = 10.0f;
}

void FountainEffect::UpdateParticle(float timeDelta, Particle & particle)
{
	static glm::vec3 gravity = glm::vec3(0, -9.8, 0);

	particle.velocity += gravity * timeDelta;
	particle.position += particle.velocity * timeDelta;

	// Fade the alpha as we approach the ground
	float fadeHeight = 5;

	particle.diffuse.a = glm::clamp<float>(particle.position.y / fadeHeight, 0.0f, 1.0f);
	if (particle.position.y < 0)
	{
		InitParticle(particle);
	}
}

float FountainEffect::RandomClamped(float min, float max)
{
	float r = (float)rand()/(float)RAND_MAX;
	float range = max - min;
	return max - (r * range); 
}
