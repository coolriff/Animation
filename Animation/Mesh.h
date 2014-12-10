#ifndef MESH_H
#define	MESH_H

#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
// GL Includes
#include <GL/glew.h> // Contains all the necessary OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include "ogldev_util.h"
#include "ogldev_texture.h"
#include "ogldev_math_3d.h"

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define NUM_BONES_PER_VEREX 5
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

using namespace std;

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool LoadMesh(string& Filename);
	void Update(GLuint modelLoc, glm::mat4 modelTransform);
	void Render();
	GLuint NumBones();

	void BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms);
	aiNode* getRootNode();

	Matrix4f rootTransform;
	Matrix4f m_GlobalInverseTransform;

private:

	struct BoneInfo
	{
		Matrix4f BoneOffset;
		Matrix4f FinalTransformation;        

		BoneInfo()
		{
			BoneOffset.SetZero();
			FinalTransformation.SetZero();            
		}
	};
	
	struct VertexBoneData
	{        
		GLuint IDs[4];
		float Weights[4];

		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			ZERO_MEM(IDs);
			ZERO_MEM(Weights);        
		}

		void AddBoneData(GLuint BoneID, float Weight);
	};
	
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
	GLuint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	GLuint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	GLuint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);

	bool InitFromScene(const aiScene* pScene, string& Filename);

	void InitMesh(GLuint MeshIndex,const aiMesh* paiMesh, 
		vector<Vertex> &vertices, 
		vector<VertexBoneData>& bones,
		vector<unsigned int>& indices);

	bool InitMaterials(const aiScene* pScene, string& Filename);

	vector<Mesh::VertexBoneData> LoadBones(GLuint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData> bones);

	void Clear();
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
#define INVALID_MATERIAL 0xFFFFFFFF

	struct MeshEntry 
	{
		MeshEntry()
		{
			NumIndices    = 0;
			BaseVertex    = 0;
			BaseIndex     = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		~MeshEntry()
		{
			if (m_VBO != 0) 
			{
				glDeleteBuffers(1, &m_VBO);
			}

			if (m_VAO != 0) 
			{
				glDeleteVertexArrays(1, &m_VAO);
				m_VAO = 0;
			}
		}

		unsigned int NumIndices;
		unsigned int MaterialIndex;
		unsigned int BaseVertex;
		unsigned int BaseIndex;

		GLuint m_VAO;
		GLuint m_VBO,vb_vbo;
		GLuint m_IBO;

		void Init(const vector<Vertex>& vertices,const vector<unsigned int>& indices);
		void Init(const vector<Vertex>& vertices,const vector<unsigned int>& indices, vector<VertexBoneData>& Bones);
		void Render();
	};

	vector<MeshEntry> m_Entries;
	vector<Texture*> m_Textures;

	
	map<string,GLuint> m_BoneMapping; // maps a bone name to its index
	GLuint m_NumBones;
	vector<BoneInfo> m_BoneInfo;


	const aiScene* m_pScene;
	Assimp::Importer m_Importer;

};

#endif;