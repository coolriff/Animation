#include "Bone.h"

Bone::Bone(void)
{
}


Bone::~Bone(void)
{
}

Bone* Bone::createBone(int ID, glm::vec3 pos, float damp_width)
{
	Bone* tempBone = new Bone();
	tempBone->id = ID;
	tempBone->pos = pos; 
	tempBone->offset = glm::translate(glm::mat4(1.0),pos);
	tempBone->localTransformation = glm::translate(glm::mat4(1.0),pos);
	tempBone->globalTransformation = glm::mat4(1);
	tempBone->damp_width = damp_width;
	return tempBone;
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

void Bone::setJointLimit(float max_rx, float min_rx, float max_ry, float min_ry, float max_rz, float min_rz)
{
	this->max_rx = max_rx;
	this->min_rx = min_rx;
	this->max_ry = max_ry;
	this->min_ry = min_ry;
	this->max_rz = max_rz;
	this->min_rz = min_rz;
}
