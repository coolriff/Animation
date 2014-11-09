#include "Skeleton.h"


Skeleton::Skeleton(void)
{
	bone = new Bone();
	lastTime = glfwGetTime();
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
	timeFlag = true;
}

Skeleton::~Skeleton(void)
{
}

void Skeleton::createHandNode()
{
	//root
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0),0.0f);

	//no 5
	handNode[1] = bone->createBone(1,glm::vec3(3.2,8,0),0.0f);
	handNode[2] = bone->createBone(2,glm::vec3(0,3,0),0.0f);
	handNode[3] = bone->createBone(3,glm::vec3(0,3,0),0.0f);

	//no 4
	handNode[4] = bone->createBone(4,glm::vec3(1.1,8,0),0.0f);
	handNode[5] = bone->createBone(5,glm::vec3(0,3,0),0.0f);
	handNode[6] = bone->createBone(6,glm::vec3(0,3,0),0.0f);

	//no 3
	handNode[7] = bone->createBone(7,glm::vec3(-1.1,8,0),0.0f);
	handNode[8] = bone->createBone(8,glm::vec3(0,3,0),0.0f);
	handNode[9] = bone->createBone(9,glm::vec3(0,3,0),0.0f);

	//no 2
	handNode[10] = bone->createBone(10,glm::vec3(-3.2,8,0),0.0f);
	handNode[11] = bone->createBone(11,glm::vec3(0,3,0),0.0f);
	handNode[12] = bone->createBone(12,glm::vec3(0,3,0),0.0f);

	//no 1
	handNode[13] = bone->createBone(13,glm::vec3(-5,4,0),0.0f);
	handNode[14] = bone->createBone(14,glm::vec3(0,3,0),0.0f);
	handNode[15] = bone->createBone(15,glm::vec3(0,3,0),0.0f);


	//add children
	handNode[0]->children[0] = handNode[1];
	handNode[0]->children[1] = handNode[4];
	handNode[0]->children[2] = handNode[7];
	handNode[0]->children[3] = handNode[10];
	handNode[0]->children[4] = handNode[13];
	handNode[0]->chilrenSize = 5;

	handNode[1]->children[0] = handNode[2];
	handNode[1]->chilrenSize = 1;
	handNode[2]->children[0] = handNode[3];
	handNode[2]->chilrenSize = 1;
	handNode[3]->chilrenSize = 0;

	handNode[4]->children[0] = handNode[5];
	handNode[4]->chilrenSize = 1;
	handNode[5]->children[0] = handNode[6];
	handNode[5]->chilrenSize = 1;
	handNode[6]->chilrenSize = 0;

	handNode[7]->children[0] = handNode[8];
	handNode[7]->chilrenSize = 1;
	handNode[8]->children[0] = handNode[9];
	handNode[8]->chilrenSize = 1;
	handNode[9]->chilrenSize = 0;

	handNode[10]->children[0] = handNode[11];
	handNode[10]->chilrenSize = 1;
	handNode[11]->children[0] = handNode[12];
	handNode[11]->chilrenSize = 1;
	handNode[12]->chilrenSize = 0;

	handNode[13]->children[0] = handNode[14];
	handNode[13]->chilrenSize = 1;
	handNode[14]->children[0] = handNode[15];
	handNode[14]->chilrenSize = 1;
	handNode[15]->chilrenSize = 0;

	//add parent
	handNode[1]->parent = handNode[0];
	handNode[4]->parent = handNode[0];
	handNode[7]->parent = handNode[0];
	handNode[10]->parent = handNode[0];
	handNode[13]->parent = handNode[0];

	handNode[2]->parent = handNode[1];
	handNode[3]->parent = handNode[2];

	handNode[5]->parent = handNode[4];
	handNode[6]->parent = handNode[5];

	handNode[8]->parent = handNode[7];
	handNode[9]->parent = handNode[8];

	handNode[11]->parent = handNode[10];
	handNode[12]->parent = handNode[11];

	handNode[14]->parent = handNode[13];
	handNode[15]->parent = handNode[14];
}


void Skeleton::drawHand(GLuint shaderProgramID)
{
	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	for(int i = 0; i < 16; i++) 
	{
		if (i == 0)
		{
			cylinder[i] = new Cylinder(8.0, 4.0, 4.0, cy_color_up, cy_color_down,2);
			cylinder[i]->generateObjectBuffer(shaderProgramID);
		}
		else
		{
			cylinder[i] = new Cylinder(3, 0.8, 0.8, cy_color_up, cy_color_down,16);
			cylinder[i]->generateObjectBuffer(shaderProgramID);
		}
	}
}


void Skeleton::updateHand(GLuint shaderProgramID)
{
	if(timeFlag)
	{
		deltaTime++;
		if (deltaTime > 30)
		{
			timeFlag = false;
		}
	}
	if (timeFlag == false)
	{
		deltaTime--;
		if (deltaTime < 0)
		{
			timeFlag = true;

		}
	}

	traverse(handNode[0], deltaTime);
	cylinder[0]->draw();

	for (int i = 1; i < 16; i++)
	{
		//handNode[i]->globalTransformation = handNode[i]->parent->globalTransformation * handNode[i]->localTransformation;
		handNode[i]->globalTransformation = calcGlobalTransformation(handNode[i]);
		cylinder[i]->update(handNode[i]->globalTransformation, shaderProgramID);
		cylinder[i]->draw();
	}
}


Bone* Skeleton::traverse(Bone* bone, float deltaTime)
{
	if (!bone)
	{
		return NULL;
	}

	if (bone->id != 0)
	{
		bone->localTransformation = glm::rotate(bone->offset, deltaTime, glm::vec3(1,0,0));
		
	}
	
	for (int i = 0; i < bone->chilrenSize; i++)
	{
		traverse(bone->children[i], deltaTime);
	}
}


glm::mat4 Skeleton::calcGlobalTransformation(Bone* bone)
{
	glm::mat4 currentGlobalTransformation;
	if (!bone)
	{
		return currentGlobalTransformation;
	}

	currentGlobalTransformation = bone->localTransformation;

	if (bone->parent)
	{
		currentGlobalTransformation = bone->parent->globalTransformation * currentGlobalTransformation;
		//bone = bone->parent;
	}
	return currentGlobalTransformation;
}
