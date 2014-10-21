#include "Bone.h"


Bone::Bone(void)
{
	createHand();
}


Bone::~Bone(void)
{
}

Bone::Bones Bone::createBone(int ID, glm::vec3 pos)
{
	Bones* tempBone = new Bone::Bones;
	tempBone->ids = ID;
	tempBone->pos = pos;
	tempBone->localTransformation = glm::translate(glm::mat4(1.0),pos)*glm::mat4(1)*glm::scale(glm::mat4(1),glm::vec3(1));
	return *tempBone;
}

void Bone::addChild(Bones* bone)
{

}

void Bone::addParent(Bone *parent)
{

}

int Bone::getID(Bones* bone)
{
	return bone->ids;
}

void Bone::createHand()
{

}

// glm::mat4 Bone::calculateGlobalTransformation(Bones* parent)
// {
// 	return glm::mat4(1);
// }


