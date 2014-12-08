#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "HumanSkeleton.h"
#include "Cylinder.h"

#include <iostream>

// Assimp includes

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

class Lab3Model
{
public:
	Lab3Model(void);
	~Lab3Model(void);

	Setup* m_setup;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;
	HumanSkeleton* humanSkeleton;
	Cylinder* plane;
	Cylinder* gate[7];

	GLuint vao;
	Assimp::Importer importer;
	glm::mat4 bone_offset_mats;
	int point_count;
	int bone_count;

	void drawGate();
	void createGate();
	void run(void);
	void initShaders();
	bool load_mesh (std::string file_name, GLuint* vao, int* point_count, glm::mat4* bone_offset_mats, int* bone_count);
	glm::mat4 convertAssimpMatrix (aiMatrix4x4 m);
};

