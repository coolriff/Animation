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
	tempBone->id = ID;
	tempBone->pos = pos; 
	tempBone->offset = glm::translate(glm::mat4(1.0),pos);
	tempBone->localTransformation = glm::translate(glm::mat4(1.0),pos);
	tempBone->globalTransformation = glm::mat4(1);
	return tempBone;
}

void Bone::addChild(Bone* child)
{
	children[chilrenSize] = child;
	chilrenSize++;
}

void Bone::addParent(Bone *parent)
{
	this->parent = parent;
}

int Bone::getID(Bone* bone)
{
	return bone->id;
}

glm::vec3 Bone::getPos(Bone* bone)
{
	return bone->pos;
}