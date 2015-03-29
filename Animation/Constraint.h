#pragma once
#include <glm/glm.hpp>
#include "particle.h"


class Constraint
{
public:

	Particle *p1, *p2; 
	float rest_distance;
	bool drawable;

	Constraint(Particle *p1, Particle *p2)
	{
		this->p1 = p1;
		this->p2 = p2;

		rest_distance = glm::length(p1->getPos()-p2->getPos());
		drawable = true;
	}

	void satisfyConstraint()
	{
		glm::vec3 p1_to_p2 = p2->getPos()-p1->getPos();
		float current_distance = glm::length(p1_to_p2);
		glm::vec3 correctionVector = p1_to_p2 * (1.0f - rest_distance/current_distance); 
		glm::vec3 correctionVectorHalf = correctionVector * 0.5f; 
		p1->offsetPos(correctionVectorHalf); 
		p2->offsetPos(-correctionVectorHalf); 
	}
};