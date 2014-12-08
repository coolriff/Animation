#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <list>

class Keyframe
{
public:
	Keyframe(void);
	~Keyframe(void);

	glm::vec3 initRootPos;
	glm::vec3 currentRootPos;
	glm::vec3 targetRootPos;

	float initJointDegree;
	float currentJointDegree;
	float targetJointDegree;

	void setInitRootPos(glm::vec3 initRootPos);
	void setCurrentRootPos(glm::vec3 currentRootPos);
	void setTargetRootPos(glm::vec3 targetRootPos);
	void setJoint(float initJointDegree, float targetJointDegree);
	void setCurrentJonit(float currentJointDegree);
};

