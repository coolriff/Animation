#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "mesh.h"
#include "skinning_technique.h"

#include <iostream>

// Assimp includes

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0
#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1
#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2
#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3
#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4

class Lab3Model
{
public:
	Lab3Model(void);
	~Lab3Model(void);

	Setup* m_setup;
	Shader* m_shader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;
	Mesh m_mesh;
	SkinningTechnique* m_pEffect;
	DirectionalLight m_directionalLight;

	GLuint vao;
	Assimp::Importer importer;
	glm::mat4 bone_offset_mats;
	int point_count;
	int bone_count;
	long long m_startTime;

	void run(void);
	void initShaders();
	bool load_mesh (std::string file_name, GLuint* vao, int* point_count, glm::mat4* bone_offset_mats, int* bone_count);
	glm::mat4 convertAssimpMatrix (aiMatrix4x4 m);
	float GetRunningTime();
};

