#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iostream>
#include <vector>
#include<list>


class Bone
{
public:
	Bone(void);
	~Bone(void);

	int ids;
	glm::vec3 pos;
	glm::mat4 localTransformation;
	std::vector<Bone> children;
	Bone* parent;

	Bone* createBone(int ID, glm::vec3 pos);
	void addChild(Bone* bone);
	void addParent(Bone *parent);
	//std::string findByID(int ID);
	int getID(Bone* bone);
	glm::vec3 getPos(Bone* bone);

	//glm::mat4 calculateGlobalTransformation(Bones* parent);
};

