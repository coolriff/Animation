#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iostream>
#include <vector>
#include <list>


class Bone
{
public:
	Bone(void);
	~Bone(void);

	int id;
	glm::vec3 pos;
	glm::mat4 offset;
	glm::mat4 localTransformation;
	glm::mat4 globalTransformation;
	Bone* children[1];
	Bone* parent;
	float damp_width;
	int chilrenSize;
	Bone* createBone(int ID, glm::vec3 pos, float damp_width);
	void addChild(Bone* child);
	void addParent(Bone* parent);
	int getID(Bone* bone);
	glm::vec3 getPos(Bone* bone);
};