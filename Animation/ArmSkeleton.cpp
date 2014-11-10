#include "ArmSkeleton.h"
#define EFFECTOR_POS 3
#define MAX_IK_TRIES 100
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
	armTargetPos = glm::vec3(10,-10,10);
	joint2TargetPos = 1000.0f;
}


ArmSkeleton::~ArmSkeleton(void)
{
}

void ArmSkeleton::createArmNode()
{
	//	handNode[3] = bone->createBone(3,glm::vec3(0,5,0),0.0f);	//effector


	handNode[0] = bone->createBone(0,glm::vec3(0,0,0),90.0f);	//upper arm
	handNode[1] = bone->createBone(1,glm::vec3(0,12,0),90.0f);	//lower arm
	handNode[2] = bone->createBone(2,glm::vec3(0,12,0),180.0f);	//wrist arm
	handNode[3] = bone->createBone(3,glm::vec3(0,5,0),0.0f);	//effector

	//1
	handNode[4] = bone->createBone(4,glm::vec3(2.4,0,0),0.0f); 
	handNode[5] = bone->createBone(5,glm::vec3(0,1.25,0),0.0f);
	handNode[6] = bone->createBone(6,glm::vec3(0,1.25,0),0.0f);

	//2
	handNode[7] = bone->createBone(7,glm::vec3(0.8,0,0),0.0f); 
	handNode[8] = bone->createBone(8,glm::vec3(0,1.5,0),0.0f);
	handNode[9] = bone->createBone(9,glm::vec3(0,1.5,0),0.0f);

	//3
	handNode[10] = bone->createBone(10,glm::vec3(-0.8,0,0),0.0f); 
	handNode[11] = bone->createBone(11,glm::vec3(0,1.75,0),0.0f);
	handNode[12] = bone->createBone(12,glm::vec3(0,1.75,0),0.0f);

	//4
	handNode[13] = bone->createBone(13,glm::vec3(-2.4,0,0),0.0f); 
	handNode[14] = bone->createBone(14,glm::vec3(0,1.5,0),0.0f);
	handNode[15] = bone->createBone(15,glm::vec3(0,1.5,0),0.0f);

	//5
	handNode[16] = bone->createBone(16,glm::vec3(-3.5,-2.5,0),0.0f); 
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

void ArmSkeleton::drawArmMesh(GLuint shaderProgramID)
{
	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	armTarget = new Cylinder(1, 0.5, 0.5, cy_color_up, cy_color_down,4);
	armTarget->generateObjectBuffer(shaderProgramID);

	cylinder[0] = new Cylinder(12, 0.4, 0.8, cy_color_up, cy_color_down,16);	//upper arm
	cylinder[0]->generateObjectBuffer(shaderProgramID);

	cylinder[1] = new Cylinder(12, 0.4, 0.8, cy_color_up, cy_color_down,16);	//lower arm
	cylinder[1]->generateObjectBuffer(shaderProgramID);

	cylinder[2] = new Cylinder(5, 3, 2, cy_color_up, cy_color_down,2);		//wrist
	cylinder[2]->generateObjectBuffer(shaderProgramID);

	cylinder[3] = new Cylinder(1, 0.5, 0.5, cy_color_up, cy_color_down,16);		//effector
	cylinder[3]->generateObjectBuffer(shaderProgramID);

	//1 finger
	cylinder[4] = new Cylinder(1.25, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[4]->generateObjectBuffer(shaderProgramID);
	cylinder[5] = new Cylinder(1.25, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[5]->generateObjectBuffer(shaderProgramID);
	cylinder[6] = new Cylinder(1.25, 0.4, 0.6, cy_color_up, cy_color_down,16);
	cylinder[6]->generateObjectBuffer(shaderProgramID);

	//2 finger
	cylinder[7] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[7]->generateObjectBuffer(shaderProgramID);
	cylinder[8] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[8]->generateObjectBuffer(shaderProgramID);
	cylinder[9] = new Cylinder(1.5, 0.4, 0.6, cy_color_up, cy_color_down,16);
	cylinder[9]->generateObjectBuffer(shaderProgramID);

	//3 finger
	cylinder[10] = new Cylinder(1.75, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[10]->generateObjectBuffer(shaderProgramID);
	cylinder[11] = new Cylinder(1.75, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[11]->generateObjectBuffer(shaderProgramID);
	cylinder[12] = new Cylinder(1.75, 0.4, 0.6, cy_color_up, cy_color_down,16);
	cylinder[12]->generateObjectBuffer(shaderProgramID);

	//4 finger
	cylinder[13] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[13]->generateObjectBuffer(shaderProgramID);
	cylinder[14] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[14]->generateObjectBuffer(shaderProgramID);
	cylinder[15] = new Cylinder(1.5, 0.4, 0.6, cy_color_up, cy_color_down,16);
	cylinder[15]->generateObjectBuffer(shaderProgramID);

	//5 finger
	cylinder[16] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[16]->generateObjectBuffer(shaderProgramID);
	cylinder[17] = new Cylinder(1.5, 0.6, 0.6, cy_color_up, cy_color_down,16);
	cylinder[17]->generateObjectBuffer(shaderProgramID);
	cylinder[18] = new Cylinder(1.5, 0.4, 0.6, cy_color_up, cy_color_down,16);
	cylinder[18]->generateObjectBuffer(shaderProgramID);
}

void ArmSkeleton::updateArmMesh(GLuint shaderProgramID)
{
	//handNode[0]->localTransformation = glm::rotate(handNode[0]->offset, 45.0f, glm::vec3(1,0,0));
	//handNode[1]->localTransformation = glm::rotate(handNode[1]->offset, 45.0f, glm::vec3(0,-1,0));



	for (int i = 0; i < HAND_NODE_NUM; i++)
	{
// 		if (handNode[i]->id == 0)
// 		{
// 			handNode[i]->globalTransformation = handNode[i]->localTransformation;
// 		}
// 		else
// 		{
// 			handNode[i]->globalTransformation = handNode[i]->parent->globalTransformation * handNode[i]->localTransformation;
// 		}
		//handNode[i]->globalTransformation = calcGlobalTransformation(handNode[i]);
		cylinder[i]->update(handNode[i]->globalTransformation, shaderProgramID);
		cylinder[i]->draw();
	}
}

void ArmSkeleton::updateArmTarget(GLuint shaderProgramID)
{
// 	static double lastTime = glfwGetTime();
// 	double currentTime = glfwGetTime();
// 	float deltaTime = float(currentTime - lastTime);

	// Move y up
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_UP ) == GLFW_PRESS){
		armTargetPos.y += 0.1;
	}

	// Move y down
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS){
		armTargetPos.y -= 0.1;
	}

	// Move x right
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
		armTargetPos.x += 0.1;
	}

	// Move x left
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS){
		armTargetPos.x -= 0.1;
	}

	// Move z forward
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_I ) == GLFW_PRESS){
		armTargetPos.z -= 0.1;
	}

	// Move z backward
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_K ) == GLFW_PRESS){
		armTargetPos.z += 0.1;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_Z ) == GLFW_PRESS){
		CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_X ) == GLFW_PRESS){
		CCDIKSolve(handNode[1], handNode[3], armTargetPos, 0);
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_C ) == GLFW_PRESS){
		CCDIKSolve(handNode[0], handNode[3], armTargetPos, 0);
	}

	armTargetTransformation = glm::translate(glm::mat4(1),glm::vec3(armTargetPos.x,armTargetPos.y-3,armTargetPos.z));
	armTarget->update(armTargetTransformation, shaderProgramID);
	armTarget->draw();
}


void ArmSkeleton::calcGlobalTransformation()
{
// 	if(timeFlag)
// 	{
// 		deltaTime++;
// 		if (deltaTime > 30)
// 		{
// 			timeFlag = false;
// 		}
// 	}
// 	if (!timeFlag)
// 	{
// 		deltaTime--;
// 		if (deltaTime < 0)
// 		{
// 			timeFlag = true;
// 
// 		}
// 	}

	if (endToTargeDistance < 2.0f)
	{
		for (int i = 4; i < 19; i++)
		{
			handNode[i]->localTransformation = glm::rotate(handNode[i]->offset, deltaTime, glm::vec3(-1,0,0));
		}
		deltaTime += 0.1f;
		if (deltaTime > 30.0f)
		{
			deltaTime = 30.0f;
		}
	}

	if (endToTargeDistance >= 2.0f)
	{
		for (int i = 4; i < 19; i++)
		{
			handNode[i]->localTransformation = glm::rotate(handNode[i]->offset, deltaTime, glm::vec3(-1,0,0));
		}
		deltaTime -= 0.1f;
		if (deltaTime < 0.0f)
		{
			deltaTime = 0.0f;
		}
	}

	handNode[16]->localTransformation = glm::rotate(handNode[16]->offset, 25.0f, glm::vec3(0,0,1));

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

bool ArmSkeleton::calculateInverseKinematics()
{
	for ( int i = 0; i < MAX_IK_TRIES; ++i )
	{
		for ( int j = 0; j < EFFECTOR_POS; ++j )
		{
			CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
			calcGlobalTransformation();
		}
		calcEffectorToTargetDistance();
		if ( endToTargeDistance <= IK_POS_THRESH)
		{
			return true;
		}
		else
		{
			continue;
		}
	}
}

void ArmSkeleton::CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum)
{
	glm::vec3 effectorPos = glm::vec3(effector->globalTransformation[3][0], effector->globalTransformation[3][1], effector->globalTransformation[3][2]);
	glm::vec3 bonePos = glm::vec3(bone->globalTransformation[3][0], bone->globalTransformation[3][1], bone->globalTransformation[3][2]);

	glm::vec3 endVector = effectorPos - bonePos;
	glm::vec3 endVectorNor = glm::normalize(endVector);

	glm::vec3 targetVector = armTargetPos - bonePos;
	glm::vec3 targetVectorNor = glm::normalize(targetVector);

	float cosAngle = glm::dot(targetVectorNor, endVectorNor);

	if (cosAngle < 0.99999 && cosAngle > -0.99999)
	{
		// USE THE CROSS PRODUCT TO CHECK WHICH WAY TO ROTATE
		float norm_u_norm_v = glm::sqrt( glm::dot(targetVector, targetVector) * glm::dot(endVector, endVector));
		float real_part = norm_u_norm_v + glm::dot(targetVector, endVector);
		glm::vec3 crossResult;
		if (real_part < 1.e-6f * norm_u_norm_v)
		{
			// If u and v are exactly opposite, rotate 180 degrees
			// around an arbitrary orthogonal axis. Axis normalization
			// can happen later, when we normalize the quaternion.
			real_part = 0.0f;
			crossResult = glm::abs(targetVectorNor.x) > glm::abs(targetVectorNor.z) ? glm::vec3(-targetVectorNor.y, targetVectorNor.x, 0.f) : glm::vec3(0.f, -targetVectorNor.z, targetVectorNor.y);
		}
		else
		{
			// Otherwise, build quaternion the standard way.
			crossResult = glm::cross(targetVectorNor, endVectorNor);
		}

		crossResult = glm::normalize(crossResult);
		float turnAngle = glm::acos(cosAngle);	// GET THE ANGLE
		float turnDeg = glm::degrees(turnAngle); // COVERT TO DEGREES

		// DAMPING
		if (turnDeg > bone->damp_width)
		{
			turnDeg = bone->damp_width;
		}

		glm::mat4 rot =  glm::toMat4(glm::angleAxis(turnDeg, crossResult));
		bone->localTransformation *= rot;

		// 				// HANDLE THE DOF RESTRICTIONS IF I WANT THEM
		// 				if (m_DOF_Restrict)
		// 					CheckDOFRestrictions(&m_Link[link]);
		// 				// RECALC ALL THE MATRICES WITHOUT DRAWING ANYTHING
		// 				drawScene(FALSE);		// CHANGE THIS TO TRUE IF YOU WANT TO SEE THE ITERATION

	}

	if (bone->parent)
	{
		CCDIKSolve(bone->parent, effector, armTargetPos, iterNum);
	}
}


void ArmSkeleton::checkDOFRestrictions(Bone* bone)
{
// 	/// Local Variables ///////////////////////////////////////////////////////////
// 	tVector		euler;		// PLACE TO STORE EULER ANGLES
// 	///////////////////////////////////////////////////////////////////////////////
// 
// 	// FIRST STEP IS TO CONVERT LINK QUATERNION BACK TO EULER ANGLES
// 	QuatToEuler(&link->quat, &euler);
// 
// 	glm::a
// 
// 	// CHECK THE DOF SETTINGS
// 	if (euler.x > (float)link->max_rx) 
// 		euler.x = (float)link->max_rx;
// 	if (euler.x < (float)link->min_rx) 
// 		euler.x = (float)link->min_rx;
// 	if (euler.y > (float)link->max_ry) 
// 		euler.y = (float)link->max_ry;
// 	if (euler.y < (float)link->min_ry) 
// 		euler.y = (float)link->min_ry;
// 	if (euler.z > (float)link->max_rz) 
// 		euler.z = (float)link->max_rz;
// 	if (euler.z < (float)link->min_rz) 
// 		euler.z = (float)link->min_rz;
// 
// 	// BACK TO QUATERNION
// 	EulerToQuaternion(&euler, &link->quat);
}

void ArmSkeleton::calcEffectorToTargetDistance()
{
	 endToTargeDistance = glm::distance(glm::vec3(handNode[3]->globalTransformation[3][0], handNode[3]->globalTransformation[3][1], handNode[3]->globalTransformation[3][2]), armTargetPos);
}
