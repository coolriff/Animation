#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"


class Lab1
{
public:
	Lab1(void);
	~Lab1(void);

	Setup* m_setup;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;

	void run(void);
	void initShaders();
};

