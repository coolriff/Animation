#pragma once
#include "Setup.h"
#include "Shader.h"
#include "ObjectBuffer.h"
#include "Camera.h"
#include "HumanSkeleton.h"
#include "Cylinder.h"
#include "ArmSkeleton.h"

#include "Mesh.h"
#include "maths_funcs.h"
#include "skinning_technique.h"
#include "ogldev_app.h"
#include "engine_common.h"

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
	Shader* m_shader_loader;
	ObjectBuffer* m_objectBuffer;
	Camera* m_camera;
	HumanSkeleton* humanSkeleton;
	Cylinder* plane;
	Cylinder* gate[8];
	Cylinder* stag[2];
	ArmSkeleton* armSkeleton;

	Mesh* m_mesh;
	SkinningTechnique* m_effect;
	

	Mesh* football_mesh;
	SkinningTechnique* football_effect;

	Mesh* floor_mesh;
	SkinningTechnique* floor_effect;

	GLuint vao;
	Assimp::Importer importer;
	glm::mat4 bone_offset_mats;

	float conterAni;

	int point_count;
	int bone_count;
	glm::vec3 animationPos;
	glm::vec3 targetPos;
	glm::vec3 pathPos;

	bool thirdCamera;
	bool thirdCamera_2;
	bool animationState;
	bool followBallCamera;
	bool drawCircleCamera;
	float startTime;
	float charRot;
	bool timeFlag;
	float deltaTime;
	bool cameraPath;
	float camTime;
	void timeKeyControl();
	void drawGate();
	void createGate();
	void run(void);
	void initShaders();
	glm::mat4 convertAssimpMatrix (Matrix4f m);

	void initLoaderShaders();
	float GetRunningTime();
	void staging();
};

