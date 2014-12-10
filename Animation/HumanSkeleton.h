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

	Bone* handNode[19];
	Cylinder* armCylinder[19];
	Cylinder* armTarget;
	glm::vec3 armTargetPos;
	glm::mat4 armTargetTransformation;
	float endToTargeDistance;

	Bone* starNode;
	Cylinder* movingStar[14];
	glm::vec3 starNodePos[14];
	Setup* m_setup;
	//Keyframe* action[15];

	glm::vec3 humanNodePos[15];
	glm::vec3 actionDegree[15];


	glm::vec3 cModelPos;
	glm::vec3 hModelPos;

	glm::mat4 hModelMat;

	float deltaTime;
	float dt;
	bool timeFlag;
	bool action_1;
	bool action_2;
	bool action_3;
	bool action_4;
	bool kick;
	bool flyingBall;
	bool walking;
	bool ballAnimation;
	bool kickStateReady;
	bool state1;
	bool state2;
	bool state3;

	bool cModel;
	bool hModel;

	bool cball;
	bool hball;

	bool hAnimation;

	float deltaY;
	float deltaX;
	float deltaZ;
	float deltaT;
	float ballToFloor;

	void createHumanNode();
	void drawHumanMesh(GLuint shaderProgramID);
	void updateHumanMesh(GLuint shaderProgramID);

	void createArmNode();
	void drawArmMesh(GLuint shaderProgramID);
	void updateArmMesh(GLuint shaderProgramID);
	void updateBallPos(GLuint shaderProgramID);
	void ballAction();
	void calculateInverseKinematics();
	void calcEffectorToTargetDistance();
	glm::quat calcJointLimit(Bone* bone, glm::vec3 angles);
	glm::vec3 interpolateCubicSpine(float counter, glm::vec3 startpoint, glm::vec3 point1,  glm::vec3 point2, glm::vec3 targetPoint);
	glm::vec3 interpolateCubic(float deltaTime, glm::vec3 beingPos, glm::vec3 point1,glm::vec3 point2, glm::vec3 endPos);

	void updateStar(GLuint shaderProgramID);
	float calcDistance(glm::vec3 pos1, glm::vec3 pos2);
	glm::vec3 slerp (glm::vec3 p1, glm::vec3 p2, float t);
	void traverse(Bone* bone, float deltaTime);
	glm::vec3 cInterpolate(float time, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
	void keyControl(GLuint shaderProgramID);
	void calcGlobalTransformation();
};

