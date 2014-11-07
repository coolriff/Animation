#include "ArmSkeleton.h"


ArmSkeleton::ArmSkeleton(Setup* m_setup)
{
	this->m_setup = m_setup;

	bone = new Bone();
	//lastTime = glfwGetTime();
	//currentTime = glfwGetTime();
	//deltaTime = float(currentTime - lastTime);
	timeFlag = true;
	speed = 15.0f;
	armTargetPos = glm::vec3(0,35,0);
	joint2TargetPos = 1000.0f;
}


ArmSkeleton::~ArmSkeleton(void)
{
}

void ArmSkeleton::createArmNode()
{
	//root
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0));
	handNode[1] = bone->createBone(1,glm::vec3(0,12,0));
	handNode[2] = bone->createBone(2,glm::vec3(0,12,0));
	handNode[3] = bone->createBone(3,glm::vec3(0,6,0));

	handNode[0]->children[0] = handNode[1];
	handNode[1]->children[0] = handNode[2];
	handNode[2]->children[0] = handNode[3];

	handNode[0]->parent = nullptr;
	handNode[1]->parent = handNode[0];
	handNode[2]->parent = handNode[1];
	handNode[3]->parent = handNode[2];

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

	//handNode[0]->localTransformation = glm::rotate(handNode[0]->offset, 30.0f, glm::vec3(1,0,0));

	for (int i = 0; i < 4; i++)
	{
		if (handNode[i]->id == 0)
		{
			handNode[i]->globalTransformation = handNode[i]->localTransformation;
		}
		else
		{
			handNode[i]->globalTransformation = handNode[i]->parent->globalTransformation * handNode[i]->localTransformation;
		}
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

// void ArmSkeleton::CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum)
// {
// 	glm::vec3 absJoint2End = effector->pos - bone->pos;
// 	glm::vec3 absJoint2Target = bone->pos - armTargetPos;
// 
// 	absJoint2End = glm::normalize(absJoint2End);
// 	absJoint2Target = glm::normalize(absJoint2Target);
// 
// 	float deltaAngle = glm::dot(absJoint2End, absJoint2Target);
// 	float rotationAngle = glm::acos(deltaAngle*360/(2 * 3.14f));
// 
// 	glm::vec3 rotateAxis = glm::cross(absJoint2Target, absJoint2End);
// 
// 	rotateAxis = glm::normalize(rotateAxis);
// 
// 	glm::quat deltaRotation;
// 
// // 	if (deltaAngle < 0.9)
// // 	{
// // 		if (rotateAxis.z >= 0)
// // 		{
// // 			deltaRotation = glm::angleAxis(rotationAngle, rotateAxis);
// // 		} 
// // 		else
// // 		{
// // 			deltaRotation = glm::angleAxis(-rotationAngle, rotateAxis);
// // 		}
// // 	}
// 
// 	deltaRotation = glm::angleAxis(rotationAngle, rotateAxis);
// 
// 	glm::mat4 quatRotationn = glm::toMat4(glm::normalize(deltaRotation));
// 
// 	bone->localTransformation *= quatRotationn;
// }


void ArmSkeleton::CCDIKSolve(Bone* bone, Bone* effector, glm::vec3 armTargetPos, int iterNum)
{
	glm::vec3 effectorPos = glm::vec3(effector->globalTransformation[3][0], effector->globalTransformation[3][1], effector->globalTransformation[3][2]);
	glm::vec3 bonePos = glm::vec3(bone->globalTransformation[3][0], bone->globalTransformation[3][1], bone->globalTransformation[3][2]);

	glm::vec3 endVector = effectorPos - bonePos;
	glm::vec3 targetVector = armTargetPos - bonePos;
	endVector = glm::normalize(endVector);
	targetVector = glm::normalize(targetVector);

	float cosAngle = (glm::dot(targetVector, endVector));
	float angle_degrees = (acos(cosAngle)) * 180 / 3.14;
	glm::vec3 rotation_Axis(glm::normalize(-glm::cross(targetVector, endVector)));

	glm::mat4 local_anim = glm::rotate(bone->offset, angle_degrees, rotation_Axis);

	if (cosAngle < 0.999 )
	{
		bone->localTransformation  =  local_anim;
	}



// 	endVector = glm::normalize(endVector);
// 	targetVector = glm::normalize(targetVector);

// 	float theta = glm::acos(glm::dot(endVector, targetVector));
// 
// 	glm::vec3 axis = glm::cross(targetVector, endVector);
// 	axis = glm::normalize(axis);
// 
// 	glm::quat deltaRotation = glm::angleAxis(glm::degrees(theta), axis);
// 
// 	//glm::mat4 quatRotationn = glm::toMat4(glm::normalize(deltaRotation));
// 
// 	glm::mat4 quatRotationn = glm::mat4_cast(deltaRotation);
// 
// 	bone->localTransformation =   quatRotationn;

// 	if (theta < 0.9 || theta > -0.9)
// 	{
// 		//bone->localTransformation = glm::rotate(bone->localTransformation, glm::degrees(rotationAngle), rotateAxis);
// 
// 		bone->localTransformation =  bone->localTransformation * quatRotationn;
// 	}

// 	if (bone->parent)
// 	{
// 		CCDIKSolve(bone->parent, effector, armTargetPos, iterNum);
// 	}

	joint2TargetPos = glm::distance(effectorPos, armTargetPos);
}

void ArmSkeleton::CalculateInverseKinematics()
{
// 	int iterationCount = 0;
// 
// 	while (iterationCount++ < 5)
// 	{
// 		CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
// 	}

	CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
	CCDIKSolve(handNode[1], handNode[3], armTargetPos, 0);
	CCDIKSolve(handNode[0], handNode[3], armTargetPos, 0);

// 	if (joint2TargetPos > 2)
// 	{
// 		CCDIKSolve(handNode[2], handNode[3], armTargetPos, 0);
// 		CCDIKSolve(handNode[1], handNode[3], armTargetPos, 0);
// 		CCDIKSolve(handNode[0], handNode[3], armTargetPos, 0);
// 	}
}
