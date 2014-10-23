#pragma once
#include "Bone.h"
#include "Cylinder.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


class Skeleton
{
public:
	Skeleton(void);
	~Skeleton(void);

	Bone* bone;
	Bone* handNode[16];
	Cylinder* cylinder[16];

	double lastTime;
	double currentTime;
	float deltaTime;
	bool timeFlag;
	float whatsoever;

	void createHandNode();
	void drawHand(GLuint shaderProgramID);
	void updateHand(GLuint shaderProgramID);
	Bone* traverse(Bone* bone, float deltaTime);
	glm::mat4 calcGlobalTransformation(Bone* bone);
};

