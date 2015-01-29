#ifndef SPHERE_H
#define SPHERE_H

#include "RigidBody.h"

class Sphere : public RigidBody
{

public:
	Sphere(glm::vec3 &p = glm::vec3(), glm::quat &o = glm::quat(), float radius = 0.5f, float d = 1.0f);
};

#endif