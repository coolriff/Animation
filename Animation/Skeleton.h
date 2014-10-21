#pragma once
#include "Bone.h"
#include "Cylinder.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Skeleton
{
public:
	Skeleton(void);
	~Skeleton(void);

	Bone* bone;
	Bone* handNode[16];
	Cylinder* cylinder[16];

	void createHandNode();
	void drawHand(GLuint shaderProgramID);
	void updateHand();
	glm::mat4 travel(Bone* bone);
	void updateCylinders();
};

