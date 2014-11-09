#include "ArmSkeleton.h"
#define EFFECTOR_POS 3
#define MAX_IK_TRIES 100
#define IK_POS_THRESH 0.5f

ArmSkeleton::ArmSkeleton(Setup* m_setup)
{
	this->m_setup = m_setup;

	bone = new Bone();
	//lastTime = glfwGetTime();
	//currentTime = glfwGetTime();
	//deltaTime = float(currentTime - lastTime);
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
	//root
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0),90.0f);
	handNode[1] = bone->createBone(1,glm::vec3(0,12,0),90.0f);
	handNode[2] = bone->createBone(2,glm::vec3(0,12,0),180.0f);
	handNode[3] = bone->createBone(3,glm::vec3(0,6,0),0.0f);

	handNode[0]->children[0] = handNode[1];
	handNode[1]->children[0] = handNode[2];
	handNode[2]->children[0] = handNode[3];

	handNode[0]->parent = nullptr;
	handNode[1]->parent = handNode[0];
	handNode[2]->parent = handNode[1];
	handNode[3]->parent = handNode[2];

	//handNode[0]->localTransformation = glm::rotate(handNode[0]->offset, 90.0f, glm::vec3(0,1,0));

	for (int i = 0 ; i < 4 ; i++)
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

	cylinder[0] = new Cylinder(12, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder[0]->generateObjectBuffer(shaderProgramID);

	cylinder[1] = new Cylinder(12, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder[1]->generateObjectBuffer(shaderProgramID);

	cylinder[2] = new Cylinder(6, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder[2]->generateObjectBuffer(shaderProgramID);

	cylinder[3] = new Cylinder(1, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder[3]->generateObjectBuffer(shaderProgramID);
}

void ArmSkeleton::updateArmMesh(GLuint shaderProgramID)
{
	//handNode[0]->localTransformation = glm::rotate(handNode[0]->offset, 45.0f, glm::vec3(1,0,0));
	//handNode[1]->localTransformation = glm::rotate(handNode[1]->offset, 45.0f, glm::vec3(0,-1,0));

	for (int i = 0; i < 4; i++)
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
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

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

// 		if (joint2TargetPos > 1.0f)
// 		{
// 			for (int i = 0 ; i < 10 ; i++)
// 			{
// 				CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
// 			}
// 		}
		CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_X ) == GLFW_PRESS){
		CCDIKSolve(handNode[1], handNode[3], armTargetPos, 0);
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_C ) == GLFW_PRESS){
		CCDIKSolve(handNode[0], handNode[3], armTargetPos, 0);
	}

	armTargetTransformation = glm::translate(glm::mat4(1),armTargetPos);
	armTarget->update(armTargetTransformation, shaderProgramID);
	armTarget->draw();
}


//working except Z==0. 
// void ArmSkeleton::CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum)
// {
// 	glm::vec3 effectorPos = glm::vec3(effector->globalTransformation[3][0], effector->globalTransformation[3][1], effector->globalTransformation[3][2]);
// 	glm::vec3 bonePos = glm::vec3(bone->globalTransformation[3][0], bone->globalTransformation[3][1], bone->globalTransformation[3][2]);
// 	glm::vec3 endVector = effectorPos - bonePos;
// 	glm::vec3 targetVector = armTargetPos - bonePos;
// 
// 	endVector = glm::normalize(endVector);
// 	targetVector = glm::normalize(targetVector);
// 
// 	float cos_theta = glm::dot(targetVector, endVector);
// 	float half_cos = glm::sqrt(0.5f * (1.0f + cos_theta));
// 	float half_sin = glm::sqrt(0.5f * (1.0f - cos_theta));
// 
// 	glm::vec3 w = glm::normalize(glm::cross(targetVector, endVector));
// 	glm::quat rot = glm::quat(half_cos, half_sin * w.x, half_sin * w.y, half_sin * w.z);
// 
// 	glm::mat4 rotMat4 = glm::toMat4(rot);
// 
// 	if (cos_theta < 0.999 && cos_theta > -0.999)
// 	{
// 		bone->localTransformation *= rotMat4;
// 	}
// 
// 	if (bone->parent)
// 	{
// 		CCDIKSolve(bone->parent, effector, armTargetPos, iterNum);
// 	}
// 
// 	endToTargeDistance = glm::distance(effectorPos, armTargetPos);
// }

//better performance
// void ArmSkeleton::CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum)
// {
// 	glm::vec3 effectorPos = glm::vec3(effector->globalTransformation[3][0], effector->globalTransformation[3][1], effector->globalTransformation[3][2]);
// 	glm::vec3 bonePos = glm::vec3(bone->globalTransformation[3][0], bone->globalTransformation[3][1], bone->globalTransformation[3][2]);
// 
// 	glm::vec3 endVector;
// 	endVector.x = effectorPos.x - bonePos.x;
// 	endVector.y = effectorPos.y - bonePos.y;
// 	endVector.z = effectorPos.z - bonePos.z;
// 	glm::vec3 endVectorNor = glm::normalize(endVector);
// 
// 	glm::vec3 targetVector;
// 	targetVector.x = armTargetPos.x - bonePos.x;
// 	targetVector.y = armTargetPos.y - bonePos.y;
// 	targetVector.z = armTargetPos.z - bonePos.z;
// 	glm::vec3 targetVectorNor = glm::normalize(targetVector);
// 
//     float norm_u_norm_v = glm::sqrt( glm::dot(targetVector, targetVector) * glm::dot(endVector, endVector));
//     float real_part = norm_u_norm_v + glm::dot(targetVector, endVector);
//     glm::vec3 w;
// 
//     if (real_part < 1.e-6f * norm_u_norm_v)
//     {
//         /* If u and v are exactly opposite, rotate 180 degrees
//          * around an arbitrary orthogonal axis. Axis normalisation
//          * can happen later, when we normalise the quaternion. */
//         real_part = 0.0f;
//         w = glm::abs(targetVector.x) > glm::abs(targetVector.z) ? glm::vec3(-targetVector.y, targetVector.x, 0.f) : glm::vec3(0.f, -targetVector.z, targetVector.y);
//     }
//     else
//     {
//         /* Otherwise, build quaternion the standard way. */
//         w = glm::cross(targetVector, endVector);
//     }
// 
// 	//testing only
// 	float cosAngle = glm::dot(targetVectorNor, endVectorNor);
// 	float angleCos = glm::acos(cosAngle);
// 	float angleDe = glm::degrees(angleCos);
// 	glm::vec3 crossResult = glm::cross(endVectorNor, targetVectorNor);
// 	crossResult = glm::normalize(crossResult);
// 	glm::mat4 testRot = glm::toMat4(glm::angleAxis(angleDe, crossResult));
// 	//testing finish
// 
//     glm::quat rot = glm::normalize(glm::quat(real_part, w.x, w.y, w.z));
// 	glm::mat4 finalRot = glm::toMat4(rot);
// 
// 	//bone->localTransformation *= finalRot;
// 	bone->localTransformation *= testRot;
// 
// 	endToTargeDistance = glm::distance(effectorPos, armTargetPos);
// 
// 	if (bone->parent)
// 	{
// 		CCDIKSolve(bone->parent, effector, armTargetPos, iterNum);
// 	}
// }



// bool ArmSkeleton::CalculateInverseKinematics()
// {
// // 	int iterationCount = 0;
// // 
// // 	CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
// // 	CCDIKSolve(handNode[1], handNode[3], armTargetPos, 0);
// // 	CCDIKSolve(handNode[0], handNode[3], armTargetPos, 0);
// 
// 	
// 	/// Local Variables ///////////////////////////////////////////////////////////
// 	glm::vec3 rootPos,curEnd,desiredEnd,targetVector,curVector,crossResult;
// 	float cosAngle,turnAngle,turnDeg;
// 	int link,tries;
// 	glm::quat aquat;
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// START AT THE LAST LINK IN THE CHAIN
// 	link = handNode[2]->id;
// 	tries = 0;						// LOOP COUNTER SO I KNOW WHEN TO QUIT
// 	do
// 	{
// 		// THE COORDS OF THE X,Y,Z POSITION OF THE ROOT OF THIS BONE IS IN THE MATRIX
// 		// TRANSLATION PART WHICH IS IN THE 12,13,14 POSITION OF THE MATRIX
// 		rootPos.x = handNode[link]->globalTransformation[3][0];
// 		rootPos.y = handNode[link]->globalTransformation[3][1];
// 		rootPos.z = handNode[link]->globalTransformation[3][2];
// 
// 		// POSITION OF THE END EFFECTOR
// 		curEnd.x = handNode[3]->globalTransformation[3][0];
// 		curEnd.y = handNode[3]->globalTransformation[3][1];
// 		curEnd.z = handNode[3]->globalTransformation[3][2];
// 
// 		// DESIRED END EFFECTOR POSITION
// 		desiredEnd.x = (float)armTargetPos.x;
// 		desiredEnd.y = (float)armTargetPos.y;
// 		desiredEnd.z = (float)armTargetPos.z;
// 
// 		// SEE IF I AM ALREADY CLOSE ENOUGH
// 		if (glm::distance(curEnd, desiredEnd) > 1.0f)
// 		{
// 			// CREATE THE VECTOR TO THE CURRENT EFFECTOR POS
// 			curVector.x = curEnd.x - rootPos.x;
// 			curVector.y = curEnd.y - rootPos.y;
// 			curVector.z = curEnd.z - rootPos.z;
// 			// CREATE THE DESIRED EFFECTOR POSITION VECTOR
// 			targetVector.x = armTargetPos.x - rootPos.x;
// 			targetVector.y = armTargetPos.y - rootPos.y;
// 			targetVector.z = armTargetPos.z - rootPos.z;
// 
// 			// NORMALIZE THE VECTORS (EXPENSIVE, REQUIRES A SQRT)
// 			curVector = glm::normalize(curVector);
// 			targetVector = glm::normalize(targetVector);
// 
// 			// THE DOT PRODUCT GIVES ME THE COSINE OF THE DESIRED ANGLE
// 			cosAngle = glm::dot(targetVector,curVector);
// 
// 			// IF THE DOT PRODUCT RETURNS 1.0, I DON'T NEED TO ROTATE AS IT IS 0 DEGREES
// 			if (cosAngle < 0.999 && cosAngle > -0.999)
// 			{
// 				// USE THE CROSS PRODUCT TO CHECK WHICH WAY TO ROTATE
// 				glm::vec3 crossResult = glm::cross(curVector, targetVector);
// 				crossResult = glm::normalize(crossResult);
// 				turnAngle = glm::acos(cosAngle);	// GET THE ANGLE
// 				turnDeg = glm::degrees(turnAngle);		// COVERT TO DEGREES
// 				// DAMPING
// 				if (turnDeg > handNode[link]->damp_width) 
// 					turnDeg = handNode[link]->damp_width;
// 
// 				glm::mat4 rot =  glm::toMat4(glm::angleAxis(turnDeg, crossResult));
// 				handNode[link]->localTransformation *= rot;
// 				if (link == 2)
// 				{
// 					handNode[3]->globalTransformation = handNode[2]->globalTransformation * handNode[3]->localTransformation;
// 				}
// 
// // 				// HANDLE THE DOF RESTRICTIONS IF I WANT THEM
// // 				if (m_DOF_Restrict)
// // 					CheckDOFRestrictions(&m_Link[link]);
// // 				// RECALC ALL THE MATRICES WITHOUT DRAWING ANYTHING
// // 				drawScene(FALSE);		// CHANGE THIS TO TRUE IF YOU WANT TO SEE THE ITERATION
// 			}
// 			if (--link < 0) link = handNode[2]->id;	// START OF THE CHAIN, RESTART
// 		}
// 		// QUIT IF I AM CLOSE ENOUGH OR BEEN RUNNING LONG ENOUGH
// 	} while (tries++ < 14 && glm::distance(curEnd, desiredEnd) > 1.0f);
// // 	if (tries == 10)
// // 		return FALSE;
// // 	else
// // 		return TRUE;
// 	for (int i = 0 ; i < 4 ; i++)
// 	{
// 		if (handNode[i]->id == 0)
// 		{
// 			handNode[i]->globalTransformation = handNode[i]->localTransformation;
// 		}
// 		else
// 		{
// 			handNode[i]->globalTransformation = handNode[i]->parent->globalTransformation * handNode[i]->localTransformation;
// 		}
// 	}
// // 	if (tries == 1)
// // 		return false;
// // 	else
// 		return true;
// }

// void ArmSkeleton::CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum)
// {
// 
// 	glm::vec3 rootPos,curEnd,desiredEnd,targetVector,curVector,crossResult;
// 	glm::quat aquat;
// 	// THE COORDS OF THE X,Y,Z POSITION OF THE ROOT OF THIS BONE IS IN THE MATRIX
// 	// TRANSLATION PART WHICH IS IN THE 12,13,14 POSITION OF THE MATRIX
// 	rootPos.x = bone->globalTransformation[3][0];
// 	rootPos.y = bone->globalTransformation[3][1];
// 	rootPos.z = bone->globalTransformation[3][2];
// 
// 	// POSITION OF THE END EFFECTOR
// 	curEnd.x = effector->globalTransformation[3][0];
// 	curEnd.y = effector->globalTransformation[3][1];
// 	curEnd.z = effector->globalTransformation[3][2];
// 
// 	// DESIRED END EFFECTOR POSITION
// 	desiredEnd.x = (float)armTargetPos.x;
// 	desiredEnd.y = (float)armTargetPos.y;
// 	desiredEnd.z = (float)armTargetPos.z;
// 
// 	// CREATE THE VECTOR TO THE CURRENT EFFECTOR POS
// 	curVector.x = curEnd.x - rootPos.x;
// 	curVector.y = curEnd.y - rootPos.y;
// 	curVector.z = curEnd.z - rootPos.z;
// 	// CREATE THE DESIRED EFFECTOR POSITION VECTOR
// 	targetVector.x = armTargetPos.x - rootPos.x;
// 	targetVector.y = armTargetPos.y - rootPos.y;
// 	targetVector.z = armTargetPos.z - rootPos.z;
// 
// 	// NORMALIZE THE VECTORS (EXPENSIVE, REQUIRES A SQRT)
// 	curVector = glm::normalize(curVector);
// 	targetVector = glm::normalize(targetVector);
// 
// 	// THE DOT PRODUCT GIVES ME THE COSINE OF THE DESIRED ANGLE
// 	float cosAngle = glm::dot(curVector,targetVector);
// 
// 	// IF THE DOT PRODUCT RETURNS 1.0, I DON'T NEED TO ROTATE AS IT IS 0 DEGREES
// 	if (cosAngle < 0.999 && cosAngle > -0.999)
// 	{
// 		// USE THE CROSS PRODUCT TO CHECK WHICH WAY TO ROTATE
// 		glm::vec3 crossResult = glm::cross(curVector, targetVector);
// 		crossResult = glm::normalize(crossResult);
// 		float turnAngle = glm::acos(cosAngle);	// GET THE ANGLE
// 		float turnDeg = glm::degrees(turnAngle);		// COVERT TO DEGREES
// 		// DAMPING
// 		if (turnDeg > bone->damp_width)
// 		{
// 			turnDeg = bone->damp_width;
// 		}
// 
// 		glm::mat4 rot =  glm::toMat4(glm::angleAxis(turnDeg, crossResult));
// 		bone->localTransformation *= rot;
// 
// 		// 				// HANDLE THE DOF RESTRICTIONS IF I WANT THEM
// 		// 				if (m_DOF_Restrict)
// 		// 					CheckDOFRestrictions(&m_Link[link]);
// 		// 				// RECALC ALL THE MATRICES WITHOUT DRAWING ANYTHING
// 		// 				drawScene(FALSE);		// CHANGE THIS TO TRUE IF YOU WANT TO SEE THE ITERATION
// 
// 	}
// 	endToTargeDistance = glm::distance(curEnd, desiredEnd);
// 	if (bone->parent)
// 	{
// 		CCDIKSolve(bone->parent, effector, armTargetPos, iterNum);
// 	}
// }

void ArmSkeleton::calcGlobalTransformation()
{
	for (int i = 0 ; i < 4 ; i++)
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

	if (cosAngle < 0.999 && cosAngle > -0.999)
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

	endToTargeDistance = glm::distance(effectorPos, armTargetPos);

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