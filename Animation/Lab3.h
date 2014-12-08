#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "Cylinder.h"
#include "Bone.h"
#include "ArmSkeleton.h"


class Lab3
{
public:
	Lab3(void);
	~Lab3(void);

	Setup* m_setup;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;
	Bone* bone;
	Cylinder* cylinder;
	ArmSkeleton* armSkeleton;


	void run(void);
	void initShaders();
};

