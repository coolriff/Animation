#include "HumanSkeleton.h"
#define EFFECTOR_POS 3
#define MAX_IK_TRIES 10
#define IK_POS_THRESH 0.5f
#define HUMAN_NODE_NUM 15
#define START_NUM 14

#define EFFECTOR_POS 3
#define MAX_IK_TRIES 10
#define IK_POS_THRESH 0.5f
#define HAND_NODE_NUM 19


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
	kick = false;
	flyingBall = false;
	walking = false;
	deltaY = 0.0f;
	deltaX = 1.0f;
	deltaZ = 0.0f;
	deltaT = 0.0f;
	ballAnimation = false;
	kickStateReady = false;
	state1 = false;
	state2 = false;
	state3 = false;
	cModel = false;
	hModel = false;
	cball = true;
	hball = false;
	hAnimation = false;
}


HumanSkeleton::~HumanSkeleton(void)
{
}

void HumanSkeleton::createHumanNode()
{
	humanNode[0] = bone->createBone(0,glm::vec3(-5,6.5,0));		humanNode[0]->bName = "Torso";
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

void HumanSkeleton::keyControl(GLuint shaderProgramID)
{
	cModelPos = glm::vec3(humanNode[13]->pos.x,humanNode[13]->pos.y,humanNode[13]->pos.z - 3);
	hModelPos = glm::vec3(hModelMat[3][0],hModelMat[3][1],hModelMat[3][2]);

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_T ) == GLFW_PRESS){
		if (!walking && cball && cModel)
		{
			kick = true;
			action_1 = true;
			flyingBall = true;
		}
		if (hball && !cball && hModel)
		{
			flyingBall = true;
			hAnimation = true;
		}
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_Z ) == GLFW_PRESS){

		cball = false;
		armTargetPos = slerp(armTargetPos,hModelPos,0.1f);

		if (glm::distance(armTargetPos, hModelPos) < 0.2)
		{
			hball = true;
			cball = false;
		}
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_X ) == GLFW_PRESS){

		hball = false;
		armTargetPos = slerp(armTargetPos,cModelPos,0.1f);

		if (glm::distance(armTargetPos, cModelPos) < 0.2)
		{
			cball = true;
			hball = false;
		}
	}



	if (flyingBall)
	{
		///Todo:: rotation for ball
		armTargetPos = cInterpolate(0.01f, armTargetPos, glm::vec3(armTargetPos.x, armTargetPos.y - 10.0f, armTargetPos.z),  glm::vec3(armTargetPos.x,armTargetPos.y - 20.0f, -80), glm::vec3(armTargetPos.x,armTargetPos.y - 0.5f,-110));
// 		glm::quat ro = glm::angleAxis(1.0f, glm::vec3(0,1,0));
// 		glm::mat4 rot =  glm::rotate()
// 		glm::quat ro = glm::toQuat(m_camera->getViewMatrix());
		if (endToTargeDistance < 2)
		{
			ballToFloor = glm::distance(glm::vec3(armTargetPos.x, 0, armTargetPos.y), armTargetPos);
			flyingBall = false;
			kickStateReady = true;
			ballAnimation = true;
		}

		if (endToTargeDistance > 90)
		{
			flyingBall = false;
			kickStateReady = true;
		}
	}

	if (walking)
	{
		//walk need to keep this 
		if(timeFlag == false)
		{
			dt += 1;

			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, dt, glm::vec3(-1,0,0));
			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, dt, glm::vec3(1,0,0));
			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, dt, glm::vec3(1,0,0));
			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, dt, glm::vec3(-1,0,0));

			if (dt > 20)
			{
				timeFlag = true;
				//walking = false;
			}
		}
		if (timeFlag == true)
		{
			dt -= 1;

			humanNode[2]->localTransformation = glm::rotate(humanNode[2]->offset, dt, glm::vec3(-1,0,0));
			humanNode[5]->localTransformation = glm::rotate(humanNode[5]->offset, dt, glm::vec3(1,0,0));
			humanNode[8]->localTransformation = glm::rotate(humanNode[8]->offset, dt, glm::vec3(1,0,0));
			humanNode[11]->localTransformation = glm::rotate(humanNode[11]->offset, dt, glm::vec3(-1,0,0));

			if (dt < -20)
			{
				timeFlag = false;
				//walking = false;
			}
		}

		if (timeFlag == false && dt == 0)
		{
			walking = false;
		}
	}

	if (ballAnimation)
	{
		if (armTargetPos.y > 0.2f && !state1 && !state2)
		{
			armTargetPos.y -= 0.3f;
			deltaY += 0.2f;
			armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x, armTargetPos.y,armTargetPos.z));
			armTargetTransformation = glm::scale(armTargetTransformation,glm::vec3(1.0f,deltaY,1.0f));
			armTarget->update(armTargetTransformation, shaderProgramID);
			armTarget->draw();
		}

		if (armTargetPos.y <= 0.2f && !state1 && !state2)
		{
			deltaY -= 0.3f;
			deltaX += 0.1f;
			armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x, armTargetPos.y,armTargetPos.z));
			armTargetTransformation = glm::scale(armTargetTransformation,glm::vec3(deltaX,deltaY,deltaX));
			armTarget->update(armTargetTransformation, shaderProgramID);
			armTarget->draw();
			if (deltaY < 0.2f)
			{
				state1 = true;
			}
		}

		if (state1)
		{
			deltaY += 0.4f;
			deltaX -= 0.3f;
			armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x, armTargetPos.y,armTargetPos.z));
			armTargetTransformation = glm::scale(armTargetTransformation,glm::vec3(deltaX,deltaY,deltaX));
			armTarget->update(armTargetTransformation, shaderProgramID);
			armTarget->draw();
			if (deltaY > 2.5f)
			{
				state2 = true;
				state1 = false;
			}
		}

		if (state2)
		{
			deltaY -= 0.2f;
			deltaX = 1.0f;
			armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x, armTargetPos.y,armTargetPos.z));
			armTargetTransformation = glm::scale(armTargetTransformation,glm::vec3(deltaX,deltaY,deltaX));
			armTarget->update(armTargetTransformation, shaderProgramID);
			armTarget->draw();
			if (deltaY < 1.0f)
			{
				deltaY = 1.0f;
				state2 = false;
				ballAnimation = false;
			}
		}
	}

	if (!kick && !ballAnimation && !flyingBall)
	{
		ballAction();
	}

	if (kick)
	{
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

			humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(humanNodePos[0].x,humanNodePos[0].y, humanNodePos[0].z - 1),0.03);
			humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);

			//glm::vec3 temp1 = slerp(glm::vec3(humanNode[0]->localTransformation[3][0], humanNode[0]->localTransformation[3][1], humanNode[0]->localTransformation[3][0]), glm::vec3(0,humanNodePos[0].y, humanNodePos[0].z - 1),0.03);
			//humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);

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

			//humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.0,-3),0.03);
			humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(humanNodePos[0].x,humanNodePos[0].y, humanNodePos[0].z - 1),0.03);
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

			//humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(0,6.5,-4),0.03);
			humanNodePos[0] = slerp(humanNodePos[0], glm::vec3(humanNodePos[0].x,humanNodePos[0].y,humanNodePos[0].z + 2),0.03);
			humanNode[0]->localTransformation = glm::translate(glm::mat4(1), humanNodePos[0]);

			if (dt > 66)
			{
				action_3 = false;
				action_1 = true;
				kick = false;
				if (kickStateReady)
				{
					kickStateReady = false;
				}
			}
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

// 	if (ballAnimation)
// 	{
// 		if (endToTargeDistance < 4.0f)
// 		{
// 			for (int i = 4; i < 19; i++)
// 			{
// 				handNode[i]->localTransformation = glm::rotate(handNode[i]->offset, deltaTime, glm::vec3(1,0,0));
// 			}
// 			deltaTime += 0.1f;
// 			if (deltaTime > 40.0f)
// 			{
// 				deltaTime = 30.0f;
// 			}
// 		}
// 
// 		if (endToTargeDistance >= 2.0f)
// 		{
// 			for (int i = 4; i < 19; i++)
// 			{
// 				handNode[i]->localTransformation = glm::rotate(handNode[i]->offset, deltaTime, glm::vec3(1,0,0));
// 			}
// 			deltaTime -= 0.1f;
// 			if (deltaTime < 0.0f)
// 			{
// 				deltaTime = 0.0f;
// 			}
// 		}
// 	}

	for (int i = 0 ; i < HAND_NODE_NUM ; i++)
	{
		if (handNode[i]->id == 0)
		{
			handNode[i]->globalTransformation = handNode[i]->localTransformation;
		}
		else
		{
			handNode[i]->globalTransformation = handNode[i]->parent->globalTransformation * handNode[i]->localTransformation;
		}
	}
}

void HumanSkeleton::createArmNode()
{
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0),0.0f);	//upper arm
	handNode[1] = bone->createBone(1,glm::vec3(0,6,0),0.0f);	//lower arm
	handNode[2] = bone->createBone(2,glm::vec3(0,6,0),0.0f);	//wrist arm
	handNode[3] = bone->createBone(3,glm::vec3(0,2.5,0),0.0f);	//effector)

	handNode[0]->setJointLimit(10.0f,-10.0f,10.0f,-10.0f,10.0f,-10.0f);
	handNode[1]->setJointLimit(10.0f,-10.0f,10.0f,-10.0f,10.0f,-10.0f);
	handNode[2]->setJointLimit(10.0f,-10.0f,10.0f,-10.0f,10.0f,-10.0f);

	// 	handNode[0]->setJointLimit(10.0f,-179.0f,90.0f,-10.0f,45.0f,-179.0f);
	// 	handNode[1]->setJointLimit(135.0f,0.0f,90.0f,0.0f,0.0f,0.0f);
	// 	handNode[2]->setJointLimit(75.0f,-75.0f,0.0f,0.0f,45.0f,-45.0f);

// 	for (int i = 0 ; i < 3 ; i++)
// 	{
// 		handNode[i]->currentXPos = 0.0f;
// 		handNode[i]->currentYPos = 0.0f;
// 		handNode[i]->currentZPos = 0.0f;
// 	}

	//1
	handNode[4] = bone->createBone(4,glm::vec3(1.2,0,0),0.0f); 
	handNode[5] = bone->createBone(5,glm::vec3(0,0.75,0),0.0f);
	handNode[6] = bone->createBone(6,glm::vec3(0,0.75,0),0.0f);

	//2
	handNode[7] = bone->createBone(7,glm::vec3(0.4,0,0),0.0f); 
	handNode[8] = bone->createBone(8,glm::vec3(0,0.87,0),0.0f);
	handNode[9] = bone->createBone(9,glm::vec3(0,0.87,0),0.0f);

	//3
	handNode[10] = bone->createBone(10,glm::vec3(-0.4,0,0),0.0f); 
	handNode[11] = bone->createBone(11,glm::vec3(0,0.75,0),0.0f);
	handNode[12] = bone->createBone(12,glm::vec3(0,0.75,0),0.0f);

	//4
	handNode[13] = bone->createBone(13,glm::vec3(-1.2,0,0),0.0f); 
	handNode[14] = bone->createBone(14,glm::vec3(0,0.625,0),0.0f);
	handNode[15] = bone->createBone(15,glm::vec3(0,0.625,0),0.0f);

	//5
	handNode[16] = bone->createBone(16,glm::vec3(1.75,-1.25,0),0.0f); 
	handNode[17] = bone->createBone(17,glm::vec3(0,0.75,0),0.0f);
	handNode[18] = bone->createBone(18,glm::vec3(0,0.75,0),0.0f);


	handNode[0]->children[0] = handNode[1];
	handNode[1]->children[0] = handNode[2];
	handNode[2]->children[0] = handNode[3];

	//1
	handNode[3]->children[0] = handNode[4]; //effector
	handNode[4]->children[0] = handNode[5];
	handNode[5]->children[0] = handNode[6];

	//2
	handNode[3]->children[0] = handNode[7]; //effector
	handNode[7]->children[0] = handNode[8];
	handNode[8]->children[0] = handNode[9];

	//3
	handNode[3]->children[0] = handNode[10]; //effector
	handNode[10]->children[0] = handNode[11];
	handNode[11]->children[0] = handNode[12];

	//4
	handNode[3]->children[0] = handNode[13]; //effector
	handNode[13]->children[0] = handNode[14];
	handNode[14]->children[0] = handNode[15];

	//5
	handNode[3]->children[0] = handNode[16]; //effector
	handNode[16]->children[0] = handNode[17];
	handNode[17]->children[0] = handNode[18];


	handNode[0]->parent = nullptr;
	handNode[1]->parent = handNode[0];
	handNode[2]->parent = handNode[1];
	handNode[3]->parent = handNode[2];

	//1
	handNode[4]->parent = handNode[3];
	handNode[5]->parent = handNode[4];
	handNode[6]->parent = handNode[5];

	//2
	handNode[7]->parent = handNode[3];
	handNode[8]->parent = handNode[7];
	handNode[9]->parent = handNode[8];

	//3
	handNode[10]->parent = handNode[3];
	handNode[11]->parent = handNode[10];
	handNode[12]->parent = handNode[11];

	//4
	handNode[13]->parent = handNode[3];
	handNode[14]->parent = handNode[13];
	handNode[15]->parent = handNode[14];

	//5
	handNode[16]->parent = handNode[3];
	handNode[17]->parent = handNode[16];
	handNode[18]->parent = handNode[17];

// 	handNode[0]->localTransformation = glm::rotate(handNode[0]->offset, 180.0f, glm::vec3(1,0,0));
 	handNode[16]->localTransformation = glm::rotate(handNode[16]->offset, 25.0f, glm::vec3(0,0,-1));
	handNode[0]->localTransformation = glm::translate(glm::mat4(1),glm::vec3(0,0,-90));

	for (int i = 0 ; i < HAND_NODE_NUM ; i++)
	{
		if (handNode[i]->id == 0)
		{
			handNode[i]->globalTransformation = handNode[i]->localTransformation;
		}
		else
		{
			handNode[i]->globalTransformation = handNode[i]->parent->globalTransformation * handNode[i]->localTransformation;
		}
	}
}

void HumanSkeleton::drawArmMesh(GLuint shaderProgramID)
{
	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	armTarget = new Cylinder(1, 0.5, 0.5, cy_color_up, cy_color_down,4);
	armTarget->generateObjectBuffer(shaderProgramID);
	ballAction();

	armCylinder[0] = new Cylinder(6, 0.25, 0.5, cy_color_up, cy_color_down,16);	//upper arm
	armCylinder[0]->generateObjectBuffer(shaderProgramID);

	armCylinder[1] = new Cylinder(6, 0.25, 0.5, cy_color_up, cy_color_down,16);	//lower arm
	armCylinder[1]->generateObjectBuffer(shaderProgramID);

	armCylinder[2] = new Cylinder(2.5, 1.5, 1, cy_color_up, cy_color_down,2);		//wrist
	armCylinder[2]->generateObjectBuffer(shaderProgramID);

	armCylinder[3] = new Cylinder(0.5, 0.25, 0.25, cy_color_up, cy_color_down,16);		//effector
	armCylinder[3]->generateObjectBuffer(shaderProgramID);

	//1 finger
	armCylinder[4] = new Cylinder(0.75, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[4]->generateObjectBuffer(shaderProgramID);
	armCylinder[5] = new Cylinder(0.75, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[5]->generateObjectBuffer(shaderProgramID);
	armCylinder[6] = new Cylinder(0.75, 0.2, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[6]->generateObjectBuffer(shaderProgramID);

	//2 finger
	armCylinder[7] = new Cylinder(0.87, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[7]->generateObjectBuffer(shaderProgramID);
	armCylinder[8] = new Cylinder(0.87, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[8]->generateObjectBuffer(shaderProgramID);
	armCylinder[9] = new Cylinder(0.87, 0.2, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[9]->generateObjectBuffer(shaderProgramID);

	//3 finger
	armCylinder[10] = new Cylinder(0.75, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[10]->generateObjectBuffer(shaderProgramID);
	armCylinder[11] = new Cylinder(0.75, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[11]->generateObjectBuffer(shaderProgramID);
	armCylinder[12] = new Cylinder(0.75, 0.2, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[12]->generateObjectBuffer(shaderProgramID);

	//4 finger
	armCylinder[13] = new Cylinder(0.625, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[13]->generateObjectBuffer(shaderProgramID);
	armCylinder[14] = new Cylinder(0.625, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[14]->generateObjectBuffer(shaderProgramID);
	armCylinder[15] = new Cylinder(0.625, 0.2, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[15]->generateObjectBuffer(shaderProgramID);

	//5 finger
	armCylinder[16] = new Cylinder(0.75, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[16]->generateObjectBuffer(shaderProgramID);
	armCylinder[17] = new Cylinder(0.75, 0.3, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[17]->generateObjectBuffer(shaderProgramID);
	armCylinder[18] = new Cylinder(0.75, 0.2, 0.3, cy_color_up, cy_color_down,16);
	armCylinder[18]->generateObjectBuffer(shaderProgramID);
}

void HumanSkeleton::updateArmMesh(GLuint shaderProgramID)
{
	for (int i = 0; i < HAND_NODE_NUM; i++)
	{
		if (i != 3)
		{
			armCylinder[i]->update(handNode[i]->globalTransformation, shaderProgramID);
			armCylinder[i]->draw();
		}
	}
}

void HumanSkeleton::updateBallPos(GLuint shaderProgramID)
{
	if (!ballAnimation)
	{
		armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x,armTargetPos.y,armTargetPos.z));
		armTarget->update(armTargetTransformation, shaderProgramID);
		armTarget->draw();
	}
}

void HumanSkeleton::ballAction()
{
	if (cball)
	{
		armTargetPos = glm::vec3(humanNode[13]->pos.x,humanNode[13]->pos.y,humanNode[13]->pos.z - 3);
	}

	if (hball)
	{
		armTargetPos = glm::vec3(hModelMat[3][0],hModelMat[3][1],hModelMat[3][2]);
	}

}

void HumanSkeleton::calculateInverseKinematics()
{
	int NumConter,linker;

	NumConter = 0;
	linker = handNode[2]->id;
	while (endToTargeDistance > IK_POS_THRESH && NumConter < MAX_IK_TRIES)
	{
		glm::vec3 effectorPos = glm::vec3(handNode[3]->globalTransformation[3][0], handNode[3]->globalTransformation[3][1], handNode[3]->globalTransformation[3][2]);
		glm::vec3 bonePos = glm::vec3(handNode[linker]->globalTransformation[3][0], handNode[linker]->globalTransformation[3][1], handNode[linker]->globalTransformation[3][2]);

		glm::vec3 endVector = effectorPos - bonePos;
		glm::vec3 endVectorNor = glm::normalize(endVector);

		glm::vec3 targetVector = armTargetPos - bonePos;
		glm::vec3 targetVectorNor = glm::normalize(targetVector);

		float cosAngle = glm::dot(targetVectorNor, endVectorNor);

		if ((endVectorNor != endVectorNor)||(targetVectorNor != targetVectorNor))
		{
			break;
		}

		if (cosAngle >= 1)
		{
			break;
		}

		if (cosAngle <= -1)
		{
			break;
		}

		glm::vec3 crossResult = glm::cross(endVectorNor, targetVectorNor);
		crossResult = glm::normalize(crossResult);
		crossResult = glm::vec3(glm::mat3(glm::inverse(handNode[linker]->globalTransformation)) * crossResult);

		float turnAngle = glm::acos(cosAngle);	// GET THE ANGLE
		float turnDeg = glm::degrees(turnAngle); // COVERT TO DEGREES

		glm::quat eulerRot = glm::angleAxis(turnDeg, crossResult);
		if (endToTargeDistance > 20)
		{
			glm::vec3 angles = glm::eulerAngles(eulerRot);
			eulerRot = calcJointLimit(handNode[linker], angles);
		}
		glm::mat4 rot =  glm::toMat4(eulerRot);

		handNode[linker]->localTransformation *= rot;

		calcGlobalTransformation();
		calcEffectorToTargetDistance();

		NumConter++;
		linker--;
		if (linker < 0)
		{
			linker = handNode[2]->id;
		}
	}
	calcEffectorToTargetDistance();
}

void HumanSkeleton::calcEffectorToTargetDistance()
{
	endToTargeDistance = glm::distance(glm::vec3(handNode[3]->globalTransformation[3][0],handNode[3]->globalTransformation[3][1], handNode[3]->globalTransformation[3][2]), armTargetPos);
}

glm::quat HumanSkeleton::calcJointLimit(Bone* bone, glm::vec3 angles)
{
	bool fx = false;
	bool fy = false;
	bool fz = false; 
	if (angles.x > 0 && bone->currentXPos == bone->max_rx)
	{
		angles.x = 0;
		fx = true;
	}
	if (angles.y > 0 && bone->currentYPos == bone->max_ry)
	{
		angles.y = 0;
		fy = true;
	}
	if (angles.z > 0 && bone->currentZPos == bone->max_rz)
	{
		angles.z = 0;
		fz = true;
	}

	if (angles.x < 0 && bone->currentXPos == bone->min_rx)
	{
		angles.x = 0;
		fx = true;
	}
	if (angles.y < 0 && bone->currentYPos == bone->min_ry)
	{
		angles.y = 0;
		fy = true;
	}
	if (angles.z < 0 && bone->currentZPos == bone->min_rz)
	{
		angles.z = 0;
		fz = true;
	}

	if((angles.x + bone->currentXPos) <= bone->max_rx && (angles.x + bone->currentXPos) >= bone->min_rx && fx == false)
	{
		bone->currentXPos = bone->currentXPos +  angles.x;
	}
	else
	{
		bool swicher = false;
		if ((angles.x + bone->currentXPos) > bone->max_rx)
		{
			angles.x = bone->max_rx - bone->currentXPos;
			bone->currentXPos = bone->max_rx;
			swicher = true;
		}

		if ((angles.x + bone->currentXPos) < bone->min_rx && swicher == false)
		{
			angles.x = bone->min_rx - bone->currentXPos;
			bone->currentXPos = bone->min_rx;
		}
	}

	if((angles.y + bone->currentYPos) <= bone->max_ry && (angles.y + bone->currentYPos) >= bone->min_ry && fy == false)
	{
		bone->currentYPos = bone->currentYPos + angles.y;
	}
	else
	{
		bool swicher = false;
		if ((angles.y + bone->currentYPos) > bone->max_ry)
		{
			angles.y = bone->max_ry - bone->currentYPos;
			bone->currentYPos = bone->max_ry;
			swicher = true;
		}


		if ((angles.y + bone->currentYPos) < bone->min_ry && swicher == false)
		{
			angles.y = bone->min_ry - bone->currentYPos;
			bone->currentYPos = bone->min_ry;
		}
	}

	if((angles.z + bone->currentZPos) <= bone->max_rz && (angles.z + bone->currentZPos) >= bone->min_rz && fz == false)
	{
		bone->currentZPos = bone->currentZPos + angles.z;
	}
	else
	{
		bool swicher = false;
		if ((angles.z + bone->currentZPos) > bone->max_rz)
		{
			angles.z = bone->max_rz - bone->currentZPos;
			bone->currentZPos = bone->max_rz;
			swicher = true;
		}

		if ((angles.z + bone->currentZPos) < bone->min_rz && swicher == false)
		{
			angles.z = bone->min_rz - bone->currentZPos;
			bone->currentZPos = bone->min_rz;
		}
	}

	angles = glm::radians(angles);

	return glm::quat(angles);
}

glm::vec3 HumanSkeleton::interpolateCubicSpine(float counter, glm::vec3 startpoint, glm::vec3 point1,  glm::vec3 point2, glm::vec3 targetPoint)
{
	//bezier curve:
	return pow(1-counter,3)*startpoint+3*counter*pow(1-counter,2)*point1+3*pow(counter,2)*(1-counter)*point2+pow(counter,3)*targetPoint;
}

glm::vec3 HumanSkeleton::interpolateCubic(float deltaTime, glm::vec3 beingPos, glm::vec3 point1,glm::vec3 point2, glm::vec3 endPos)
{
	glm::vec3 p1,p2,p3,p4;

	p1 = point2 - endPos - point1 + beingPos;
	p2 = point1 - beingPos - p1;
	p3 = endPos - point1;
	p4 = beingPos;

	return (p1 * pow(deltaTime,3) + p2 * pow(deltaTime,2) + p3 * pow(deltaTime,1) + p4);
}