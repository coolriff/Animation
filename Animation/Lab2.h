#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "Cylinder.h"
#include "Bone.h"


class Lab2
{
public:
	Lab2(void);
	~Lab2(void);

	Setup* m_setup;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;
	Cylinder* cylinder_1;
	Cylinder* cylinder_2;
	Cylinder* cylinder_3;
	Cylinder* cylinder_4;
	Cylinder* cylinder_5;
	Cylinder* cylinder_6;
	Cylinder* cylinder_7;
	Cylinder* cylinder_8;
	Cylinder* cylinder_9;
	Cylinder* cylinder_10;
	Cylinder* cylinder_11;
	Cylinder* cylinder_12;
	Cylinder* cylinder_13;
	Cylinder* cylinder_14;
	Cylinder* cylinder_15;
	Cylinder* cylinder_16;
	Bone* bone;


	void run(void);
	void initShaders();
	void createBones();
};

