#include "HumanSkeleton.h"
#define EFFECTOR_POS 3
#define MAX_IK_TRIES 10
#define IK_POS_THRESH 0.5f
#define HUMAN_NODE_NUM 15
#define START_NUM 14


HumanSkeleton::HumanSkeleton(Setup* m_setup)
{
	this->m_setup = m_setup;
	bone = new Bone();
	deltaTime = 0.0f;
	dt = 0.0f;
	timeFlag = false;
	action_1 = false;
	action_2 = false;
	action_3 = false;
	action_4 = false;
}


HumanSkeleton::~HumanSkeleton(void)
{
}

void HumanSkeleton::createHumanNode()
{
// 	humanNode[0] = bone->createBone(0,glm::vec3(0,6.5,0));		humanNode[0]->bName = "Torso";
// 	humanNode[1] = bone->createBone(1,glm::vec3(0,1,0));		humanNode[1]->bName = "Head";
// 	humanNode[2] = bone->createBone(2,glm::vec3(-1.2,0,0));		humanNode[2]->bName = "Left-upper-arm";
// 	humanNode[3] = bone->createBone(3,glm::vec3(0,-1.5,0));		humanNode[3]->bName = "Left-lower-arm";
// 	humanNode[4] = bone->createBone(4,glm::vec3(0,-1.5,0));		humanNode[4]->bName = "Left-wrist";
// 	humanNode[5] = bone->createBone(5,glm::vec3(1.2,0,0));		humanNode[5]->bName = "Right-upper-arm";
// 	humanNode[6] = bone->createBone(6,glm::vec3(0,-1.5,0));		humanNode[6]->bName = "Right-lower-arm";
// 	humanNode[7] = bone->createBone(7,glm::vec3(0,-1.5,0));		humanNode[7]->bName = "Right-wrist";
// 	humanNode[8] = bone->createBone(8,glm::vec3(-0.5,-2.5,0));	humanNode[8]->bName = "Left-upper-leg";
// 	humanNode[9] = bone->createBone(9,glm::vec3(0,-2,0));		humanNode[9]->bName = "Left-lower-leg";
// 	humanNode[10] = bone->createBone(10,glm::vec3(0,-2,0));		humanNode[10]->bName = "Left-foot";
// 	humanNode[11] = bone->createBone(11,glm::vec3(0.5,-2.5,0));	humanNode[11]->bName = "Right-upper-leg";
// 	humanNode[12] = bone->createBone(12,glm::vec3(0,-2,0));		humanNode[12]->bName = "Right-lower-leg";
// 	humanNode[13] = bone->createBone(13,glm::vec3(0,-2,0));		humanNode[13]->bName = "Right-foot";
// 	humanNode[14] = bone->createBone(14,glm::vec3(0,-2,0));		humanNode[14]->bName = "Pelvis";

	humanNode[0] = bone->createBone(0,glm::vec3(0,6.5,0));		humanNode[0]->bName = "Torso";
	humanNode[1] = bone->createBone(1,glm::vec3(0,1,0));		humanNode[1]->bName = "Head";
	humanNode[2] = bone->createBone(2,glm::vec3(-1.2,0,0));		humanNode[2]->bName = "Left-upper-arm";
	humanNode[3] = bone->createBone(3,glm::vec3(0,-1.5,0));		humanNode[3]->bName = "Left-lower-arm";
	humanNode[4] = bone->createBone(4,glm::vec3(0,-1.5,0));		humanNode[4]->bName = "Left-wrist";
	humanNode[5] = bone->createBone(5,glm::vec3(1.2,0,0));		humanNode[5]->bName = "Right-upper-arm";
	humanNode[6] = bone->createBone(6,glm::vec3(0,-1.5,0));		humanNode[6]->bName = "Right-lower-arm";
	humanNode[7] = bone->createBone(7,glm::vec3(0,-1.5,0));		humanNode[7]->bName = "Right-wrist";
	humanNode[8] = bone->createBone(8,glm::vec3(-0.5,-2.5,0));	humanNode[8]->bName = "Left-upper-leg";
	humanNode[9] = bone->createBone(9,glm::vec3(0,-2,0));		humanNode[9]->bName = "Left-lower-leg";
	humanNode[10] = bone->createBone(10,glm::vec3(0,-2,0));		humanNode[10]->bName = "Left-foot";
	humanNode[11] = bone->createBone(11,glm::vec3(0.5,-2.5,0));	humanNode[11]->bName = "Right-upper-leg";
	humanNode[12] = bone->createBone(12,glm::vec3(0,-2,0));		humanNode[12]->bName = "Right-lower-leg";
	humanNode[13] = bone->createBone(13,glm::vec3(0,-2,0));		humanNode[13]->bName = "Right-foot";
	humanNode[14] = bone->createBone(14,glm::vec3(0,-2,0));		humanNode[14]->bName = "Pelvis";

	humanNode[0]->children[0] = humanNode[1];
	humanNode[0]->children[1] = humanNode[2];	humanNode[2]->children[0] = humanNode[3];	humanNode[3]->children[0] = humanNode[4];
	humanNode[0]->children[2] = humanNode[5];	humanNode[5]->children[0] = humanNode[6];	humanNode[6]->children[0] = humanNode[7];
	humanNode[0]->children[3] = humanNode[8];	humanNode[8]->children[0] = humanNode[9];	humanNode[9]->children[0] = humanNode[10];
	humanNode[0]->children[4] = humanNode[11];	humanNode[11]->children[0] = humanNode[12];	humanNode[12]->children[0] = humanNode[13];
	humanNode[0]->children[5] = humanNode[14];

	humanNode[0]->parent = nullptr;
	humanNode[1]->parent = humanNode[0];
	humanNode[2]->parent = humanNode[0];
	humanNode[3]->parent = humanNode[2];
	humanNode[4]->parent = humanNode[3];
	humanNode[5]->parent = humanNode[0];
	humanNode[6]->parent = humanNode[5];
	humanNode[7]->parent = humanNode[6];
	humanNode[8]->parent = humanNode[0];
	humanNode[9]->parent = humanNode[8];
	humanNode[10]->parent = humanNode[9];
	humanNode[11]->parent = humanNode[0];
	humanNode[12]->parent = humanNode[11];
	humanNode[13]->parent = humanNode[12];
	humanNode[14]->parent = humanNode[0];

	for (int i = 0 ; i < HUMAN_NODE_NUM ; i++)
	{
		if (humanNode[i]->id == 0)
		{
			humanNode[i]->globalTransformation = humanNode[i]->localTransformation;
		}
		else
		{
			humanNode[i]->globalTransformation = humanNode[i]->parent->globalTransformation * humanNode[i]->localTransformation;
		}
	}

	//starNode->pos = humanNode[0]->pos;

	humanNodePos[0] = glm::vec3(0,6.5,0);

}

void HumanSkeleton::drawHumanMesh(GLuint shaderProgramID)
{
	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	for (int i = 0 ; i < HUMAN_NODE_NUM ; i++)
	{
		cylinder[i] = new Cylinder(0.2, 0.1, 0.1, cy_color_up, cy_color_down,16);	cylinder[i]->generateObjectBuffer(shaderProgramID);
	}

	glm::vec4 star_color_up = glm::vec4(1.0, 0.9, 0.0, 1.0);
	glm::vec4 star_color_down = glm::vec4(1.0, 0.9, 0.0, 1.0);

	for (int i = 0 ; i < START_NUM ; i++)
	{
		//movingStar[i] = new Cylinder(0.1, 0.05, 0.05, star_color_up, star_color_down,16);	movingStar[i]->generateObjectBuffer(shaderProgramID);
		movingStar[i] = new Cylinder(0.1, 0.05, 0.05, cy_color_up, cy_color_down,16);	movingStar[i]->generateObjectBuffer(shaderProgramID);
	}

}

void HumanSkeleton::updateHumanMesh(GLuint shaderProgramID)
{
	//calcGlobalTransformation();

	for (int i = 0; i < HUMAN_NODE_NUM; i++)
	{
		if (humanNode[i]->id == 0)
		{
			humanNode[i]->globalTransformation = humanNode[i]->localTransformation;
		}
		else
		{
			humanNode[i]->globalTransformation = humanNode[i]->parent->globalTransformation * humanNode[i]->localTransformation;
		}

		cylinder[i]->update(humanNode[i]->globalTransformation, shaderProgramID);
		cylinder[i]->draw();
		humanNodePos[i] = glm::vec3(humanNode[i]->globalTransformation[3][0], humanNode[i]->globalTransformation[3][1], humanNode[i]->globalTransformation[3][2]);
		humanNode[i]->pos = humanNodePos[i];
	}
}

void HumanSkeleton::updateStar(GLuint shaderProgramID)
{
	deltaTime += 0.01f;

	if (deltaTime > 1.5)
	{
		deltaTime = 0.0f;
	}

	starNodePos[0] = cInterpolate(deltaTime, humanNodePos[0], humanNodePos[1], humanNodePos[0], humanNodePos[1]);
	starNodePos[1] = cInterpolate(deltaTime, humanNodePos[0], humanNodePos[2], humanNodePos[0], humanNodePos[2]);
	starNodePos[2] = cInterpolate(deltaTime, humanNodePos[0], humanNodePos[5], humanNodePos[0], humanNodePos[5]);
	starNodePos[3] = cInterpolate(deltaTime, humanNodePos[14], humanNodePos[8], humanNodePos[14], humanNodePos[8]);
	starNodePos[4] = cInterpolate(deltaTime, humanNodePos[14], humanNodePos[11], humanNodePos[14], humanNodePos[11]);
	starNodePos[5] = cInterpolate(deltaTime, humanNodePos[0], humanNodePos[14], humanNodePos[0], humanNodePos[14]);

	starNodePos[6] = cInterpolate(deltaTime, humanNodePos[2], humanNodePos[3], humanNodePos[2], humanNodePos[3]);
	starNodePos[7] = cInterpolate(deltaTime, humanNodePos[5], humanNodePos[6], humanNodePos[5], humanNodePos[6]);
	starNodePos[8] = cInterpolate(deltaTime, humanNodePos[8], humanNodePos[9], humanNodePos[8], humanNodePos[9]);
	starNodePos[9] = cInterpolate(deltaTime, humanNodePos[11], humanNodePos[12], humanNodePos[11], humanNodePos[12]);

	starNodePos[10] = cInterpolate(deltaTime, humanNodePos[3], humanNodePos[4], humanNodePos[3], humanNodePos[4]);
	starNodePos[11] = cInterpolate(deltaTime, humanNodePos[6], humanNodePos[7], humanNodePos[6], humanNodePos[7]);
	starNodePos[12] = cInterpolate(deltaTime, humanNodePos[9], humanNodePos[10], humanNodePos[9], humanNodePos[10]);
	starNodePos[13] = cInterpolate(deltaTime, humanNodePos[12], humanNodePos[13], humanNodePos[12], humanNodePos[13]);

	for (int i = 0 ; i < START_NUM ; i++)
	{
		glm::mat4 star = glm::translate(glm::mat4(1), starNodePos[i]);
		movingStar[i]->update(star, shaderProgramID);
		movingStar[i]->draw();
	}
}

float HumanSkeleton::calcDistance(glm::vec3 pos1, glm::vec3 pos2)
{
	float dis = glm::distance(pos1, pos2);
	return dis;
}

glm::vec3 HumanSkeleton::slerp (glm::vec3 p1, glm::vec3 p2, float t)
{
	//(1 - t)p1 + tp2

	glm::vec3 mDest; 
	mDest.x = ((1 - t) * p1.x) + (t * p2.x);
	mDest.y = ((1 - t) * p1.y) + (t * p2.y);
	mDest.z = ((1 - t) * p1.z) + (t * p2.z);

// 	glm::vec3 mDest; 
// 	mDest.x = srcLocation.x + (destLocation.x - srcLocation.x) * Time;
// 	mDest.y = srcLocation.y + (destLocation.y - srcLocation.y) * Time;
// 	mDest.z = srcLocation.z + (destLocation.z - srcLocation.z) * Time;
	return mDest;
}

void HumanSkeleton::traverse(Bone* bone, float deltaTime)
{
	if (!bone)
	{
		//
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

glm::vec3 HumanSkeleton::cInterpolate(float time, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
{
	glm::vec3 startPoint = p1;
	glm::vec3 point1 = p2;
	glm::vec3 point2 = p3;
	glm::vec3 endPoint = p4;

	glm::vec3 a0,a1,a2,a3;


	a0 = point2 - endPoint - point1 + startPoint;
	a1 = point1 - startPoint - a0;
	a2 = endPoint - point1;
	a3 = startPoint;

	return(a0 * pow(time,3) + a1 * pow(time,2) + a2 * pow(time,1) + a3);
}

void HumanSkeleton::keyControl()
{
	//Move y up
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_UP ) == GLFW_PRESS){
// 		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.5,0),0.1);
// 		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);
		action_1 = true;
	}

	// Move y down
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS){
// 		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.5,0),0.1);
// 		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);
		action_1 = false;
	}

	// Move x right
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
		humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, 20.0f, glm::vec3(-1,0,0));
		humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, 20.0f, glm::vec3(1,0,0));
		humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, 20.0f, glm::vec3(1,0,0));
		humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, 20.0f, glm::vec3(-1,0,0));

// 		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6,-3),0.1);
// 		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);
	}

	// Move x left
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS){
		humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, 20.0f, glm::vec3(1,0,0));
		humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, 20.0f, glm::vec3(-1,0,0));
		humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, 20.0f, glm::vec3(-1,0,0));
		humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, 20.0f, glm::vec3(1,0,0));

// 		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.5,-6),0.1);
// 		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_T ) == GLFW_PRESS){
		action_1 = true;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_F ) == GLFW_PRESS){
		action_1 = false;
	}

//		//walk need to keep this 
// 		if(timeFlag == false)
// 		{
// 			dt += 0.5;
// 
// 			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, dt, glm::vec3(-1,0,0));
// 			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, dt, glm::vec3(1,0,0));
// 			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, dt, glm::vec3(1,0,0));
// 			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, dt, glm::vec3(-1,0,0));
// 
// 			if (dt > 20)
// 			{
// 				timeFlag = true;
// 			}
// 		}
// 		if (timeFlag == true)
// 		{
// 			dt -= 0.5;
// 
// 			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, dt, glm::vec3(-1,0,0));
// 			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, dt, glm::vec3(1,0,0));
// 			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, dt, glm::vec3(1,0,0));
// 			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, dt, glm::vec3(-1,0,0));
// 
// 			if (dt < -20)
// 			{
// 				timeFlag = false;
// 
// 			}
// 		}

	if (action_1)
	{
		dt += 1;

		if (dt < 10)
		{
			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, dt, glm::vec3(-1,0,0));
			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, dt, glm::vec3(0,0,1));
		}

		if (dt < 15)
		{
			humanNode[1]->localTransformation = glm::rotate(humanNode[1]->offset, dt, glm::vec3(-1,0,0));
		}

		if (dt < 25)
		{
			humanNode[9]->localTransformation = glm::rotate(humanNode[9]->offset, dt, glm::vec3(-1,0,0));
			humanNode[12]->localTransformation = glm::rotate(humanNode[12]->offset, dt, glm::vec3(-1,0,0));
		}

		if (dt < 40)
		{
			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, dt, glm::vec3(-1,0,0));
		}

		if (dt < 45)
		{
			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, dt, glm::vec3(1,0,0));
			humanNode[6]->localTransformation = glm::rotate(humanNode[6]->offset, dt, glm::vec3(1,0,0));
		}

		if (dt < 50)
		{
			float temp = dt * 2;
			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, temp, glm::vec3(0,0,-1));
		}

		if (dt > 50)
		{
			action_1 = false;
			action_2 = true;
			dt = 0;
			actionDegree[1].x = -15;
			actionDegree[2].x = -10;
			actionDegree[2].z = -100;
			actionDegree[5].x = -10;
			actionDegree[5].z = 10;
			actionDegree[6].x = 45;
			actionDegree[8].x = 45;
			actionDegree[9].x = -25;
			actionDegree[11].x = -40;
			actionDegree[12].x = -25;
		}

		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,5.5,-3),0.03);
		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);
	}
	if (action_2)
	{
		dt += 1;

		if (dt < 45) // x 35
		{
			humanNode[3]->localTransformation = glm::rotate(humanNode[3]->offset, dt, glm::vec3(1,0,0));
		}

		if (dt < 45) // z 10
		{
			actionDegree[2].z += 2;
			float temp = actionDegree[2].z;
			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, temp, glm::vec3(0,0,1));
		}

		if (dt < 53) // x 65 
		{
			float temp = dt * 2;
// 			actionDegree[11].x += 1;
// 			float temp = actionDegree[11].x;
			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 55) // x -45
		{
			actionDegree[5].x += 1;
			float temp = actionDegree[5].x;
			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, temp, glm::vec3(-1,0,0));
		}

		if (dt < 25) // x 20
		{
			actionDegree[6].x -= 1;
			float temp = actionDegree[6].x;
			humanNode[6]->localTransformation = glm::rotate(humanNode[6]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 65) // x -20
		{
			actionDegree[8].x -= 1;
			float temp = actionDegree[8].x;
			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt > 66)
		{
			dt = 0;
			action_2 = false;
			action_3 = true;
			actionDegree[3].x = 35;
			actionDegree[2].x = 35;
			actionDegree[2].z = 10;
			actionDegree[5].x = -45;
			actionDegree[6].x = 20;
			actionDegree[8].x = -20;
			actionDegree[11].x = 65;
		}

		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.0,-3),0.03);
		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);
	}

	if (action_3)
	{
		dt += 1;

		if (dt < 16)
		{
			actionDegree[1].x += 1;
			float temp = actionDegree[1].x;
			humanNode[1]->localTransformation = glm::rotate(humanNode[1]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 26)
		{
			actionDegree[12].x += 1;
			float temp = actionDegree[12].x;
			humanNode[12]->localTransformation = glm::rotate(humanNode[12]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 66)
		{
			actionDegree[11].x -= 1;
			float temp = actionDegree[11].x;
			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 26)
		{
			actionDegree[9].x += 1;
			float temp = actionDegree[9].x;
			humanNode[9]->localTransformation = glm::rotate(humanNode[9]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 21)
		{
			actionDegree[8].x += 1;
			float temp = actionDegree[8].x;
			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, temp, glm::vec3(1,0,0));
		}


		if (dt < 11)
		{
			actionDegree[2].z -= 1;
			float temp = actionDegree[2].z;
			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, temp, glm::vec3(0,0,1));
		}

		if (dt < 36)
		{
			actionDegree[3].x -= 1;
			float temp = actionDegree[3].x;
			humanNode[3]->localTransformation = glm::rotate(humanNode[3]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 46)
		{
			actionDegree[5].x += 1;
			float temp = actionDegree[5].x;
			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, temp, glm::vec3(1,0,0));
		}

		if (dt < 21)
		{
			actionDegree[6].x -= 1;
			float temp = actionDegree[6].x;
			humanNode[6]->localTransformation = glm::rotate(humanNode[6]->offset, temp, glm::vec3(1,0,0));
		}

		humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.5,-4),0.03);
		humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);

		if (dt > 66)
		{
			action_3 = false;
		}
	}
}

void HumanSkeleton::calcGlobalTransformation()
{
	for (int i = 0 ; i < HUMAN_NODE_NUM ; i++)
	{
		if (humanNode[i]->id == 0)
		{
			humanNode[i]->globalTransformation = humanNode[i]->localTransformation;
		}
		else
		{
			humanNode[i]->globalTransformation = humanNode[i]->parent->globalTransformation * humanNode[i]->localTransformation;
		}
	}
}

void HumanSkeleton::penaltyAction_1(float dt)
{

}

void HumanSkeleton::createAction()
{

}