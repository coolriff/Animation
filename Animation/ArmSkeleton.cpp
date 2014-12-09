#include "ArmSkeleton.h"
#define EFFECTOR_POS 3
#define MAX_IK_TRIES 10
#define IK_POS_THRESH 0.5f
#define HAND_NODE_NUM 19

ArmSkeleton::ArmSkeleton(Setup* m_setup)
{
	this->m_setup = m_setup;

	bone = new Bone();
	lastTime = glfwGetTime();
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
	timeFlag = true;
	speed = 15.0f;
	armTargetPos = glm::vec3(0,-29,0);
	joint2TargetPos = 1000.0f;
	deltaT = 0;
	targetLoops = false;
}


ArmSkeleton::~ArmSkeleton(void)
{
}


void ArmSkeleton::createArmNode()
{
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0),0.0f);	//upper arm
	handNode[1] = bone->createBone(1,glm::vec3(0,12,0),0.0f);	//lower arm
	handNode[2] = bone->createBone(2,glm::vec3(0,12,0),0.0f);	//wrist arm
	handNode[3] = bone->createBone(3,glm::vec3(0,5,0),0.0f);	//effector)

// 	handNode[0]->setJointLimit(10.0f,-10.0f,10.0f,-10.0f,10.0f,-10.0f);
//  handNode[1]->setJointLimit(10.0f,-10.0f,10.0f,-10.0f,10.0f,-10.0f);
//  handNode[2]->setJointLimit(10.0f,-10.0f,10.0f,-10.0f,10.0f,-10.0f);

// 	handNode[0]->setJointLimit(10.0f,-179.0f,90.0f,-10.0f,45.0f,-179.0f);
// 	handNode[1]->setJointLimit(135.0f,0.0f,90.0f,0.0f,0.0f,0.0f);
// 	handNode[2]->setJointLimit(75.0f,-75.0f,0.0f,0.0f,45.0f,-45.0f);

	for (int i = 0 ; i < 3 ; i++)
	{
		handNode[i]->currentXPos = 0.0f;
		handNode[i]->currentYPos = 0.0f;
		handNode[i]->currentZPos = 0.0f;
	}

	//1
	handNode[4] = bone->createBone(4,glm::vec3(2.4,0,0),0.0f); 
	handNode[5] = bone->createBone(5,glm::vec3(0,1.5,0),0.0f);
	handNode[6] = bone->createBone(6,glm::vec3(0,1.5,0),0.0f);

	//2
	handNode[7] = bone->createBone(7,glm::vec3(0.8,0,0),0.0f); 
	handNode[8] = bone->createBone(8,glm::vec3(0,1.75,0),0.0f);
	handNode[9] = bone->createBone(9,glm::vec3(0,1.75,0),0.0f);

	//3
	handNode[10] = bone->createBone(10,glm::vec3(-0.8,0,0),0.0f); 
	handNode[11] = bone->createBone(11,glm::vec3(0,1.5,0),0.0f);
	handNode[12] = bone->createBone(12,glm::vec3(0,1.5,0),0.0f);

	//4
	handNode[13] = bone->createBone(13,glm::vec3(-2.4,0,0),0.0f); 
	handNode[14] = bone->createBone(14,glm::vec3(0,1.25,0),0.0f);
	handNode[15] = bone->createBone(15,glm::vec3(0,1.25,0),0.0f);

	//5
	handNode[16] = bone->createBone(16,glm::vec3(3.5,-2.5,0),0.0f); 
	handNode[17] = bone->createBone(17,glm::vec3(0,1.5,0),0.0f);
	handNode[18] = bone->createBone(18,glm::vec3(0,1.5,0),0.0f);


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

	handNode[0]->localTransformation = glm::rotate(handNode[0]->offset, 180.0f, glm::vec3(1,0,0));
	handNode[16]->localTransformation = glm::rotate(handNode[16]->offset, 25.0f, glm::vec3(0,0,-1));


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

	calcEffectorToTargetDistance();
}


void ArmSkeleton::drawArmMesh(GLuint shaderProgramID)
{
	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	armTarget = new Cylinder(1, 0.5, 0.5, cy_color_up, cy_color_down,4);
	armTarget->generateObjectBuffer(shaderProgramID);

	armCylinder[0] = new Cylinder(12, 0.5, 1, cy_color_up, cy_color_down,16);	//upper arm
	armCylinder[0]->generateObjectBuffer(shaderProgramID);

	armCylinder[1] = new Cylinder(12, 0.5, 1, cy_color_up, cy_color_down,16);	//lower arm
	armCylinder[1]->generateObjectBuffer(shaderProgramID);

	armCylinder[2] = new Cylinder(5, 3, 2, cy_color_up, cy_color_down,2);		//wrist
	armCylinder[2]->generateObjectBuffer(shaderProgramID);

	armCylinder[3] = new Cylinder(1, 0.5, 0.5, cy_color_up, cy_color_down,16);		//effector
	armCylinder[3]->generateObjectBuffer(shaderProgramID);

	//1 finger
	armCylinder[4] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[4]->generateObjectBuffer(shaderProgramID);
	armCylinder[5] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[5]->generateObjectBuffer(shaderProgramID);
	armCylinder[6] = new Cylinder(1.5, 0.4, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[6]->generateObjectBuffer(shaderProgramID);

	//2 finger
	armCylinder[7] = new Cylinder(1.75, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[7]->generateObjectBuffer(shaderProgramID);
	armCylinder[8] = new Cylinder(1.75, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[8]->generateObjectBuffer(shaderProgramID);
	armCylinder[9] = new Cylinder(1.75, 0.4, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[9]->generateObjectBuffer(shaderProgramID);

	//3 finger
	armCylinder[10] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[10]->generateObjectBuffer(shaderProgramID);
	armCylinder[11] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[11]->generateObjectBuffer(shaderProgramID);
	armCylinder[12] = new Cylinder(1.5, 0.4, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[12]->generateObjectBuffer(shaderProgramID);

	//4 finger
	armCylinder[13] = new Cylinder(1.25, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[13]->generateObjectBuffer(shaderProgramID);
	armCylinder[14] = new Cylinder(1.25, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[14]->generateObjectBuffer(shaderProgramID);
	armCylinder[15] = new Cylinder(1.25, 0.4, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[15]->generateObjectBuffer(shaderProgramID);

	//5 finger
	armCylinder[16] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[16]->generateObjectBuffer(shaderProgramID);
	armCylinder[17] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[17]->generateObjectBuffer(shaderProgramID);
	armCylinder[18] = new Cylinder(1.5, 0.4, 0.6, cy_color_up, cy_color_down,16);
	armCylinder[18]->generateObjectBuffer(shaderProgramID);
}


void ArmSkeleton::updateArmMesh(GLuint shaderProgramID)
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


void ArmSkeleton::updateArmTarget(GLuint shaderProgramID)
{
// 	//Move y up
// 	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_UP ) == GLFW_PRESS){
// 		armTargetPos.y += 0.3;
// 	}
// 
// 	// Move y down
// 	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS){
// 		armTargetPos.y -= 0.3;
// 	}
// 
// 	// Move x right
// 	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
// 		armTargetPos.x += 0.3;
// 	}
// 
// 	// Move x left
// 	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS){
// 		armTargetPos.x -= 0.3;
// 	}
// 
// 	// Move z forward
// 	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_I ) == GLFW_PRESS){
// 		armTargetPos.z -= 0.3;
// 	}
// 
// 	// Move z backward
// 	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_K ) == GLFW_PRESS){
// 		armTargetPos.z += 0.3;
// 	}
// 
	deltaT+=0.004;
	if(deltaT >= 1)
	{
		deltaT = 0;
	}

	if (targetLoops == false)
	{
		armTargetPos = interpolateCubic(deltaT, glm::vec3(20,50,15), glm::vec3(130,0,15), glm::vec3(-130,0,15), glm::vec3(-30,10,10));

		if (armTargetPos == glm::vec3(-30,10,10))
		{
			targetLoops = true;
		}
	}

	if (targetLoops == true)
	{
		armTargetPos = interpolateCubic(deltaT, glm::vec3(-30,10,10), glm::vec3(-20,-10,10),
			glm::vec3(15,-20,-10), glm::vec3(20,50,15));

		if (armTargetPos == glm::vec3(20,50,15))
		{
			targetLoops = false;
		}
	}

	armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x,armTargetPos.y-3,armTargetPos.z));
	armTarget->update(armTargetTransformation, shaderProgramID);
	armTarget->draw();
}


glm::vec3 ArmSkeleton::interpolateCubic(float deltaTime, glm::vec3 beingPos, glm::vec3 point1,glm::vec3 point2, glm::vec3 endPos)
{
	glm::vec3 p1,p2,p3,p4;

	p1 = point2 - endPos - point1 + beingPos;
	p2 = point1 - beingPos - p1;
	p3 = endPos - point1;
	p4 = beingPos;

	return (p1 * pow(deltaTime,3) + p2 * pow(deltaTime,2) + p3 * pow(deltaTime,1) + p4);
}

glm::vec3 ArmSkeleton::interpolateCubic_2(float deltaTime, glm::vec3 beingPos,glm::vec3 point1,  glm::vec3 point2, glm::vec3 endPos)
{
	return pow(1-deltaTime,3)*beingPos+3*deltaTime*pow(1-deltaTime,2)*point1+3*pow(deltaTime,2)*(1-deltaTime)*point2+pow(deltaTime,3)*endPos;
}


void ArmSkeleton::calcGlobalTransformation()
{
	if (endToTargeDistance < 2.0f)
	{
		for (int i = 4; i < 19; i++)
		{
			handNode[i]->localTransformation = glm::rotate(handNode[i]->offset, deltaTime, glm::vec3(1,0,0));
		}
		deltaTime += 0.1f;
		if (deltaTime > 40.0f)
		{
			deltaTime = 30.0f;
		}
	}

	if (endToTargeDistance >= 2.0f)
	{
		for (int i = 4; i < 19; i++)
		{
			handNode[i]->localTransformation = glm::rotate(handNode[i]->offset, deltaTime, glm::vec3(1,0,0));
		}
		deltaTime -= 0.1f;
		if (deltaTime < 0.0f)
		{
			deltaTime = 0.0f;
		}
	}

	handNode[16]->localTransformation = glm::rotate(handNode[16]->offset, 25.0f, glm::vec3(0,0,-1));

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


void ArmSkeleton::calculateInverseKinematics()
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
 		glm::vec3 angles = glm::eulerAngles(eulerRot);
 		eulerRot = calcJointLimit(handNode[linker], angles);
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


glm::quat ArmSkeleton::calcJointLimit(Bone* bone, glm::vec3 angles)
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


void ArmSkeleton::calcEffectorToTargetDistance()
{
	 endToTargeDistance = glm::distance(glm::vec3(handNode[3]->globalTransformation[3][0],handNode[3]->globalTransformation[3][1], handNode[3]->globalTransformation[3][2]), armTargetPos);
}