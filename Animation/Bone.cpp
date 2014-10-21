#include "Bone.h"


Bone::Bone(void)
{
}


Bone::~Bone(void)
{
}

Bone* Bone::createBone(int ID, glm::vec3 pos)
{
	Bone* tempBone = new Bone();
	tempBone->ids = ID;
	tempBone->pos = pos;
	tempBone->localTransformation = glm::translate(glm::mat4(1.0),pos)*glm::mat4(1)*glm::scale(glm::mat4(1),glm::vec3(1));
	return tempBone;
}

void Bone::addChild(Bone* bone)
{

}

void Bone::addParent(Bone *parent)
{

}

int Bone::getID(Bone* bone)
{
	return bone->ids;
}

glm::vec3 Bone::getPos(Bone* bone)
{
	return bone->pos;
}

// glm::mat4 Bone::calculateGlobalTransformation(Bones* parent)
// {
// 	return glm::mat4(1);
// }


