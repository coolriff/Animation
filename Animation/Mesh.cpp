#include <assert.h>

#include "mesh.h"

void Mesh::VertexBoneData::AddBoneData(GLuint BoneID, float Weight)
{
    for (GLuint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) 
	{
        if (Weights[i] == 0.0) 
		{
            IDs[i]     = BoneID;
            Weights[i] = Weight;
            return;
        }        
    }
   
}

Mesh::Mesh()
{
    m_NumBones = 0;
    m_pScene = NULL;
}


Mesh::~Mesh()
{
    Clear();
}


void Mesh::Clear()
{
    for (GLuint i = 0 ; i < m_Textures.size() ; i++)
	{
        SAFE_DELETE(m_Textures[i]);
    }
}


bool Mesh::LoadMesh(string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;    
  
    m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (m_pScene) 
	{  
		m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
		rootTransform = m_pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();
        Ret = InitFromScene(m_pScene, Filename);

    }
    else 
	{
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
    }

    return Ret;
}


bool Mesh::InitFromScene(const aiScene* pScene, string& Filename)
{  
	cout << "Num Animations: " << pScene->mNumAnimations << endl;
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);    

	GLuint NumVertices = 0;
	GLuint NumIndices = 0;

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<VertexBoneData> bones;

	// Count the number of vertices and indices
	for (GLuint i = 0 ; i < m_Entries.size() ; i++) 
	{
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
		m_Entries[i].NumIndices    = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex    = NumVertices;
		m_Entries[i].BaseIndex     = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices  += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	vertices.reserve(NumVertices);
	bones.resize(NumVertices);
	indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
    for (GLuint i = 0 ; i < m_Entries.size() ; i++) 
	{
        const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, vertices, bones, indices);
    }

    if (!InitMaterials(pScene, Filename)) 
	{
        return false;
    }

}

void Mesh::InitMesh(GLuint MeshIndex, const aiMesh* paiMesh, vector<Vertex> &vertices, 
					vector<VertexBoneData>& bones,
					vector<unsigned int>& indices)
{

	// Populate the vertex attribute vectors
	for (GLuint i = 0 ; i < paiMesh->mNumVertices ; i++) 
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = paiMesh->mVertices[i].x;
		vector.y = paiMesh->mVertices[i].y;
		vector.z = paiMesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = paiMesh->mNormals[i].x;
		vector.y = paiMesh->mNormals[i].y;
		vector.z = paiMesh->mNormals[i].z;
		vertex.Normal = vector;

		if(paiMesh->HasTextureCoords(0))
		{
			glm::vec2 vec;

			vec.x = paiMesh->mTextureCoords[0][i].x;
			vec.y = paiMesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	
	if(paiMesh->HasBones())
	{
		bones = LoadBones(MeshIndex, paiMesh, bones);
	}
	
	for(GLuint i = 0; i < paiMesh->mNumFaces; i++)
	{
		aiFace face = paiMesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for(GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if(paiMesh->HasBones())
	{
		m_Entries[MeshIndex].Init(vertices, indices,bones);
	}
	else
	{
		m_Entries[MeshIndex].Init(vertices, indices);
	}
}


vector<Mesh::VertexBoneData> Mesh::LoadBones(GLuint MeshIndex, const aiMesh* pMesh, vector<VertexBoneData> bones)
{ 
    for (GLuint i = 0 ; i < pMesh->mNumBones ; i++) 
	{    
        GLuint BoneIndex = 0;        
        string BoneName(pMesh->mBones[i]->mName.data);
        
        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) 
		{
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;            
	        BoneInfo bi;			
			m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;            
            m_BoneMapping[BoneName] = BoneIndex;
        }
        else 
		{
            BoneIndex = m_BoneMapping[BoneName];
        }                      
        
        for (GLuint j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++) 
		{
            GLuint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight  = pMesh->mBones[i]->mWeights[j].mWeight;  
            bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    } 
	return bones;
}

bool Mesh::InitMaterials(const aiScene* pScene, string& Filename)
{
    // Extract the directory part from the file name
    string::size_type SlashIndex = Filename.find_last_of("/");
    string Dir;

    if (SlashIndex == string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (GLuint i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                string p(Path.data);
                
                if (p.substr(0, 2) == ".\\") {                    
                    p = p.substr(2, p.size() - 2);
                }
                               
                string FullPath = Dir + "/" + p;
                    
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) 
				{
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
                }
            }
        }
    }
    return Ret;
}

void Mesh::Render()
{
    for (GLuint i = 0 ; i < m_Entries.size() ; i++) 
	{	
		const GLuint MaterialIndex = m_Entries[i].MaterialIndex;
      
        if (m_Textures[MaterialIndex]) 
		{
            m_Textures[MaterialIndex]->Bind((GLuint)GL_TEXTURE0);
        }	

		m_Entries[i].Render();
    }
}

GLuint Mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
	for (GLuint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) 
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	assert(0);

	return 0;
}


GLuint Mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (GLuint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) 
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	assert(0);

	return 0;
}


GLuint Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (GLuint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


void Mesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	GLuint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	GLuint NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void Mesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	GLuint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	GLuint NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}


void Mesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) 
	{
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	GLuint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	GLuint NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void Mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
{    
	string NodeName(pNode->mName.data);
	const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	Matrix4f NodeTransformation(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
	
	if (pNodeAnim) 
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		Matrix4f ScalingM;
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);        
		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}
	
	Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) 
	{
		GLuint BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (GLuint i = 0 ; i < pNode->mNumChildren ; i++) 
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}


void Mesh::BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms)
{
	Matrix4f Identity;
	Identity.InitIdentity();

	float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	for (GLuint i = 0 ; i < m_NumBones ; i++) 
	{
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
	
}


const aiNodeAnim* Mesh::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
	for (GLuint i = 0 ; i < pAnimation->mNumChannels ; i++) 
	{
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

GLuint Mesh::NumBones()
{
	return m_NumBones;
}

void Mesh::Update(GLuint modelLoc, glm::mat4 modelTransform)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelTransform[0][0]);
}

void Mesh::MeshEntry::Init(const vector<Vertex>& vertices,const vector<unsigned int>& indices, vector<VertexBoneData>& Bones)
{
	glGenVertexArrays(1,&this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glGenBuffers(1, &this->vb_vbo);
	glGenBuffers(1, &this->m_IBO);

	glBindVertexArray(this->m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER,this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	glBindBuffer(GL_ARRAY_BUFFER,this->vb_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::MeshEntry::Init(const vector<Vertex>& vertices,const vector<unsigned int>& indices)
{
	glGenVertexArrays(1,&this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glGenBuffers(1, &this->m_IBO);

	glBindVertexArray(this->m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER,this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::MeshEntry::Render()
{
	glBindVertexArray(this->m_VAO);
	glDrawElementsBaseVertex(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * BaseIndex), 
		BaseVertex);
	glBindVertexArray(0);	
}
