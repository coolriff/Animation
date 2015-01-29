#include "Sphere.h"

Sphere::Sphere(glm::vec3 &p, glm::quat &o, float radius, float d) : RigidBody(p, o, glm::vec3(2.0f * radius))
{
	float mass = d * 4.0f / 3.0f * radius * radius * radius;
	m_massInverse = 1.0f / mass;

	glm::mat3 I = (2.0f * mass / 5.0f) * (radius * radius) * glm::mat3(
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1)
	);

	m_inverseI = glm::inverse(I);
}