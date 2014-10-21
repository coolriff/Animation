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
	Bone* bone;

	Bone* handNode[16];
	Cylinder* cylinder[16];

	void run(void);
	void initShaders();
	void createHandNode();
};

