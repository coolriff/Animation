#pragma once
#include "Bone.h"
#include "Cylinder.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Setup.h"
#include "glm/gtx/quaternion.hpp"


class ArmSkeleton
{
public:
	ArmSkeleton(Setup* m_setup);
	~ArmSkeleton(void);

	Bone* bone;
	Bone* handNode[4];
	Cylinder* cylinder[4];
	Cylinder* armTarget;
	Setup* m_setup;

	double lastTime;
	double currentTime;
	float deltaTime;
	bool timeFlag;
	float whatsoever;
	float speed;
	glm::vec3 armTargetPos;
	glm::mat4 armTargetTransformation;
	float joint2TargetPos;

	void createArmNode();
	void drawArmMesh(GLuint shaderProgramID);
	void updateArmMesh(GLuint shaderProgramID);
	void updateArmTarget(GLuint shaderProgramID);
	void CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum);
	void CalculateInverseKinematics();
};

