#include "Box.h"

Box::Box(glm::vec3 &p, glm::quat &o, glm::vec3 &s, float d) : RigidBody(p, o, s)
{
	float mass = d * scale.x * scale.y * scale.z;
	massInverse = 1.0f / mass;

	glm::mat3 I = mass/12.0f * glm::mat3(
		glm::vec3(scale.y * scale.y + scale.z * scale.z, 0, 0),
		glm::vec3(0, scale.x * scale.x + scale.z * scale.z, 0),
		glm::vec3(0, 0, scale.x * scale.x + scale.y * scale.y)
		);
	inverseI = glm::inverse(I);
	//inverseI = I._inverse();
}