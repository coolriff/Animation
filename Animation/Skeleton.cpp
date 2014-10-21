#include "Skeleton.h"


Skeleton::Skeleton(void)
{
	bone = new Bone();
}


Skeleton::~Skeleton(void)
{
}

void Skeleton::createHandNode()
{
	//root
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0));

	//no 5
	handNode[1] = bone->createBone(1,glm::vec3(3.2,8,0));
	handNode[2] = bone->createBone(2,glm::vec3(3.2,11,0));
	handNode[3] = bone->createBone(3,glm::vec3(3.2,14,0));

	//no 4
	handNode[4] = bone->createBone(4,glm::vec3(1.1,8,0));
	handNode[5] = bone->createBone(5,glm::vec3(1.1,11,0));
	handNode[6] = bone->createBone(6,glm::vec3(1.1,14,0));

	//no 3
	handNode[7] = bone->createBone(7,glm::vec3(-1.1,8,0));
	handNode[8] = bone->createBone(8,glm::vec3(-1.1,11,0));
	handNode[9] = bone->createBone(9,glm::vec3(-1.1,14,0));

	//no 2
	handNode[10] = bone->createBone(10,glm::vec3(-3.2,8,0));
	handNode[11] = bone->createBone(11,glm::vec3(-3.2,11,0));
	handNode[12] = bone->createBone(12,glm::vec3(-3.2,14,0));

	//no 1
	handNode[13] = bone->createBone(13,glm::vec3(-5,4,0));
	handNode[14] = bone->createBone(14,glm::vec3(-5,7,0));
	handNode[15] = bone->createBone(15,glm::vec3(-5,10,0));


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


	handNode[4]->children[0] = handNode[5];
	handNode[4]->chilrenSize = 1;
	handNode[5]->children[0] = handNode[6];
	handNode[5]->chilrenSize = 1;

	handNode[7]->children[0] = handNode[8];
	handNode[7]->chilrenSize = 1;
	handNode[8]->children[0] = handNode[9];
	handNode[8]->chilrenSize = 1;

	handNode[10]->children[0] = handNode[11];
	handNode[10]->chilrenSize = 1;
	handNode[11]->children[0] = handNode[12];
	handNode[11]->chilrenSize = 1;

	handNode[13]->children[0] = handNode[14];
	handNode[13]->chilrenSize = 1;
	handNode[14]->children[0] = handNode[15];
	handNode[14]->chilrenSize = 1;


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
}

void Skeleton::drawHand(GLuint shaderProgramID)
{
	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	for(int i = 0; i < 16; i++) 
	{
		if (i == 0)
		{
			cylinder[i] = new Cylinder(glm::vec4(bone->getPos(handNode[i]),0), 8.0, 4.0, 4.0, cy_color_up, cy_color_down,2);
			cylinder[i]->generateObjectBuffer(shaderProgramID);
		}
		else
		{
			cylinder[i] = new Cylinder(glm::vec4(bone->getPos(handNode[i]),0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
			cylinder[i]->generateObjectBuffer(shaderProgramID);
		}
	}
}

void Skeleton::updateCylinders()
{
/*	/ *cylinder[0]->vertex_positions = glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(1,0,0* /));*/
}

void Skeleton::updateHand(GLuint modelLoc)
{

	for (int i = 0; i < 16; i++)
	{
		if(i == 0)
		{
			glm::mat4 boneModelMatrix = glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(1,0,0));
			cylinder[i]->update(modelLoc,boneModelMatrix);
			cylinder[i]->draw();
		}
		else
		{
			glm::mat4 boneModelMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1,0,0));
			cylinder[i]->update(modelLoc,boneModelMatrix);
			cylinder[i]->draw();
		}
	}
}

// void Skeleton::travel(Bone* bone, glm::mat4 updatedModelMatrix)
// {
// 	int idNum = bone->getID(bone);
// 	if (idNum == 0)
// 	{
// 		glm::mat4 currentaLocalTransformation = bone->localTransformation;
// 		glm::mat4 currentTransform = bone->localTransformation;
// 	}
// 	if (idNum > 0)
// 	{
// 		glm::mat4 currentaLocalTransformation = bone->parent->localTransformation;
// 		glm::mat4 currentTransform = bone->localTransformation;
// 
// 		
// 
// 	}
// 
// 
// 
// 	if (idNum > -1)
// 	{
// 		return bone->localTransformation;
// 	}
// 	else
// 	{
// 		for (int i = 0; i < bone->chilrenSize; i++)
// 		{
// 			Bone *temp = bone->children[i];
// 			travel(temp);
// 		}
// 
// 
// 		return pGlobaSpace*currentTransform;
// 	}
// }



//skeleton_animate (
//	monkey_root_node,
//	identity_mat4 (),
//	monkey_bone_offset_matrices,
//	monkey_bone_animation_mats
//	);
