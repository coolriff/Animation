#ifndef BOX_H
#define BOX_H

#include "RigidBody.h"

class Box : public RigidBody
{

public:
	Box(glm::vec3 &p = glm::vec3(), glm::quat &o = glm::quat(), glm::vec3 &s = glm::vec3(1,1,1), float d = 1.0f);

};

#endif