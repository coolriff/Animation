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

	void createHandNode();
	void drawHand(GLuint shaderProgramID);
	void updateHand(GLuint modelLoc, GLuint shaderProgramID);
	//void travel(Bone* bone, glm::mat4 updatedModelMatrix);
	void travel(Bone* bone, GLuint shaderProgramID);
	glm::mat4 calcGlobalTransformation(Bone* bone);
};

