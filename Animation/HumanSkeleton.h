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
#include "Keyframe.h"

class HumanSkeleton
{
public:
	HumanSkeleton(Setup* m_setup);
	~HumanSkeleton(void);

	Bone* bone;
	Bone* humanNode[15];
	Cylinder* cylinder[15];
	Cylinder* armTarget;
	Bone* starNode;
	Cylinder* movingStar[14];
	glm::vec3 starNodePos[14];
	Setup* m_setup;
	//Keyframe* action[15];

	glm::vec3 humanNodePos[15];
	glm::vec3 actionDegree[15];

	float deltaTime;
	float dt;
	bool timeFlag;
	bool action_1;
	bool action_2;
	bool action_3;
	bool action_4;

	void createHumanNode();
	void drawHumanMesh(GLuint shaderProgramID);
	void updateHumanMesh(GLuint shaderProgramID);
	void updateStar(GLuint shaderProgramID);
	float calcDistance(glm::vec3 pos1, glm::vec3 pos2);
	glm::vec3 slerp (glm::vec3 p1, glm::vec3 p2, float t);
	void traverse(Bone* bone, float deltaTime);
	glm::vec3 cInterpolate(float time, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
	void keyControl();
	void calcGlobalTransformation();
	void penaltyAction_1(float dt);
	void createAction();
};

