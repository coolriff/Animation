#include "Sphere.h"

Sphere::Sphere(glm::vec3 &p, glm::quat &o, glm::vec3 &s, float d) : RigidBody(p, o, 2.0f * s)
{
	float mass = d * 4.0f / 3.0f * m_scale.x * m_scale.y * m_scale.z;
	m_massInverse = 1.0f / mass;

	glm::mat3 I = mass / 5.0f * glm::mat3(
		glm::vec3(m_scale.y * m_scale.y + m_scale.z * m_scale.z, 0, 0),
		glm::vec3(0, m_scale.x * m_scale.x + m_scale.z * m_scale.z, 0),
		glm::vec3(0, 0, m_scale.x * m_scale.x + m_scale.y * m_scale.y)
		);

	m_inverseI = glm::inverse(I);
}