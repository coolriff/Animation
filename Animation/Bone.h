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

	struct Bones
	{
		int ids;
		glm::vec3 pos;
		glm::mat4 localTransformation;
		std::list<Bones> children;
		Bones* parent;
	};

	std::list<Bones>* allBones;

	Bones createBone(int ID, glm::vec3 pos);
	void addChild(Bones* bone);
	void addParent(Bone *parent);
	int findByID(int ID);
	int getID(Bones* bone);
	void createHand();


	//glm::mat4 calculateGlobalTransformation(Bones* parent);
};

