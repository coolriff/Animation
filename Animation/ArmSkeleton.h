#pragma once
#include "Bone.h"
#include "Cylinder.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Setup.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>


class ArmSkeleton
{
public:
	ArmSkeleton(Setup* m_setup);
	~ArmSkeleton(void);

	Bone* bone;
	Bone* handNode[19];
	Cylinder* cylinder[19];
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
	float endToTargeDistance;
	float tempX,tempY,tempZ;
	float DOF_x,DOF_y,DOF_z;
	float deltaT;


	void createArmNode();
	void drawArmMesh(GLuint shaderProgramID);
	void updateArmMesh(GLuint shaderProgramID);
	void updateArmTarget(GLuint shaderProgramID);
	void calculateInverseKinematics();
	void calcGlobalTransformation();
	glm::quat checkDOFRestrictions(Bone* bone, glm::quat rot);
	void calcEffectorToTargetDistance();
	glm::vec3 interpolateCubic(float deltaTime, glm::vec3 beingPos, glm::vec3 point1,  glm::vec3 point2, glm::vec3 endPos);
};

