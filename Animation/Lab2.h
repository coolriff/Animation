#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "Cylinder.h"
#include "Bone.h"
#include "Skeleton.h"

class Lab2
{
public:
	Lab2(void);
	~Lab2(void);

	Setup* m_setup;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;
	Bone* bone;
	Skeleton* skeleton;
	Cylinder* cylinder;


	void run(void);
	void initShaders();

};

