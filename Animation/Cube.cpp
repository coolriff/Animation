#include "Cube.h"

Cube::Cube(glm::vec3 &p, glm::quat &o, glm::vec3 &s, float d) : RigidBody(p, o, s)
{
	float mass = d * m_scale.x * m_scale.y * m_scale.z;
	m_massInverse = 1.0f / mass;


	glm::mat3 I = mass/12.0f * glm::mat3(
		glm::vec3(m_scale.y * m_scale.y + m_scale.z * m_scale.z, 0, 0),
		glm::vec3(0, m_scale.x * m_scale.x + m_scale.z * m_scale.z, 0),
		glm::vec3(0, 0, m_scale.x * m_scale.x + m_scale.y * m_scale.y)
		);
	m_inverseI = glm::inverse(I);
}