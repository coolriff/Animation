#include "Keyframe.h"


Keyframe::Keyframe(void)
{
}


Keyframe::~Keyframe(void)
{
}

void Keyframe::setInitRootPos(glm::vec3 initRootPos)
{
	this->initRootPos = initRootPos;
}

void Keyframe::setCurrentRootPos(glm::vec3 currentRootPos)
{
	this->currentRootPos = currentRootPos;
}

void Keyframe::setTargetRootPos(glm::vec3 targetRootPos)
{
	this->targetRootPos = targetRootPos;
}

void Keyframe::setJoint(float initJointDegree, float targetJointDegree)
{
	this->initJointDegree = initJointDegree;
	this->targetJointDegree = targetJointDegree;
}

void Keyframe::setCurrentJonit(float currentJointDegree)
{
	this->currentJointDegree = currentJointDegree;
}

