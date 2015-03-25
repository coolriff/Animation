#include "CreateMesh.h"

#define CUBE 36
#define PI 3.1415926


CreateMesh::CreateMesh(void)
{
	isTextured = false;
}

CreateMesh::~CreateMesh(void)
{

}

void CreateMesh::createCubeMesh()
{
	glm::vec3 v[] = {
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3( 0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f)
	};

	for (int i=0; i<CUBE; i++ )
	{
		vertices.push_back(v[i]);
	}

	glm::vec4 c[] = {
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f)
	};

	for (int i=0; i<CUBE; i++ )
	{
		colors.push_back(c[i]);
		greenColors.push_back(glm::vec4(0.0f,  1.0f,  0.0f, 1.0f));
		redColors.push_back(glm::vec4(1.0f,  0.0f,  0.0f, 1.0f));
		blueColors.push_back(glm::vec4(0.0f,  0.0f,  1.0f, 1.0f));
	}

	for(unsigned int i=0; i<CUBE; i+=3)
	{
		glm::vec3 v1 = v[i+1] - v[i];
		glm::vec3 v2 = v[i+2] - v[i];

		glm::vec3 n = glm::normalize(glm::cross(v1, v2));
		normals.push_back(n);
		normals.push_back(n);
		normals.push_back(n);
	}

}

void CreateMesh::createSphereMesh(int numSegments)
{
	std::vector< glm::vec3 > v;
	std::vector< glm::vec3 > n;

	std::vector<glm::vec3> points;
	for (int j= 0; j < numSegments; ++j)
	{
		float theta = (glm::pi<float>()*j)/(numSegments);

		for( int i=0; i<numSegments; ++i)
		{
			float phi = (2*glm::pi<float>()*i)/(numSegments);
			points.push_back(glm::vec3(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta)) / 2.0f);
		}
	}

	for (int j= 0; j < numSegments; j ++)
	{ 

		for( int i=0; i<numSegments; i++ )
		{
			int iNext = i+1;
			if (i == numSegments - 1)
				iNext = 0;

			int index = (j*numSegments*6)+(i*6);

			v.push_back(points[j*numSegments+i]);
			v.push_back(points[j*numSegments+iNext]);

			if (j != numSegments -1)
				v.push_back(points[((j+1)*numSegments)+i]);
			else
				v.push_back(glm::vec3( 0, 0, -0.5f));

			n.push_back(v[index]);
			n.push_back(v[index+1]);
			n.push_back(v[index+2]);

			v.push_back(v[index+2]);
			v.push_back(v[index+1]);

			if (j != numSegments - 1)
				v.push_back(points[((j+1)*numSegments)+iNext]);
			else
				v.push_back(glm::vec3( 0,0,-0.5f));

			n.push_back(v[index+3]);
			n.push_back(v[index+4]);
			n.push_back(v[index+5]);
		}
	}

	std::vector<glm::vec4> c;
	glm::vec4 color = glm::vec4(float(rand())/RAND_MAX * 0.5f + 0.5f, float(rand())/RAND_MAX * 0.5f + 0.5f, float(rand())/RAND_MAX * 0.5f + 0.5f, 1.0f);
	for(unsigned int i=0; i<v.size(); ++i)
	{
		c.push_back(color);
	}

	vertices = v;
	colors = c;
	normals = n;
}

void CreateMesh::createBoundingSphereMesh(float radius, int resolution)
{
	float X1,Y1,X2,Y2,Z1,Z2;
	float inc1,inc2,inc3,inc4,inc5,Radius1,Radius2;

	for(int w = 0; w < resolution; w++) 
	{
		for(int h = (-resolution/2); h < (resolution/2); h++)
		{

			inc1 = (w/(float)resolution)*2*PI;
			inc2 = ((w+1)/(float)resolution)*2*PI;

			inc3 = (h/(float)resolution)*PI;
			inc4 = ((h+1)/(float)resolution)*PI;

			X1 = glm::sin(inc1);
			Y1 = glm::cos(inc1);
			X2 = glm::sin(inc2);
			Y2 = glm::cos(inc2);

			// store the upper and lower radius, remember everything is going to be drawn as triangles
			Radius1 = radius*glm::cos(inc3);
			Radius2 = radius*glm::cos(inc4);

			Z1 = radius*glm::sin(inc3); 
			Z2 = radius*glm::sin(inc4);

			// insert the triangle coordinates
			vertices.push_back(glm::vec3(Radius1*X1,Z1,Radius1*Y1));
			vertices.push_back(glm::vec3(Radius1*X2,Z1,Radius1*Y2));
			vertices.push_back(glm::vec3(Radius2*X2,Z2,Radius2*Y2));

			vertices.push_back(glm::vec3(Radius1*X1,Z1,Radius1*Y1));
			vertices.push_back(glm::vec3(Radius2*X2,Z2,Radius2*Y2));
			vertices.push_back(glm::vec3(Radius2*X1,Z2,Radius2*Y1));

			//indexVBO(v, t, n, indices, indexed_vertices, indexed_uvs, indexed_normals);	 
		}
	}

	for (int i=0; i<vertices.size(); i++ )
	{
		colors.push_back(glm::vec4(0.85f,  0.85f,  0.85f, 1.0f));
		redColors.push_back(glm::vec4(1.0f,  0,  0, 1.0f));
	}

	for(unsigned int i=0; i<vertices.size(); i+=3)
	{
		glm::vec3 v1 = vertices[i+1] - vertices[i];
		glm::vec3 v2 = vertices[i+2] - vertices[i];

		glm::vec3 n = glm::normalize(glm::cross(v1, v2));
		normals.push_back(n);
		normals.push_back(n);
		normals.push_back(n);
	}
}

void CreateMesh::setColors(glm::vec4 c)
{
	for(int i = 0; i < vertices.size(); i++)
	{
		colors.push_back(c);
	}

	vSize = vertices.size() * sizeof(glm::vec3);
	cSize = vertices.size() * sizeof(glm::vec4);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, v_VBO);
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));

	glBindVertexArray(0);
}

void CreateMesh::loadMesh(const char* filename)
{
	Assimp::Importer importer;
	aiMesh *mesh;

	const aiScene *scene = importer.ReadFile(filename, 	aiProcess_Triangulate
		| aiProcess_OptimizeGraph
		| aiProcess_OptimizeMeshes
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_CalcTangentSpace);

	if(!scene) 
	{
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	mesh = scene->mMeshes[0];
	numElements = mesh->mNumFaces * 3;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	if(mesh->HasPositions())
	{
		for(int i = 0; i < mesh->mNumVertices; i++) 
		{
			vertices.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));			
			colors.push_back(glm::vec4(0.85f,0.85f,0.85f,1));
		}

		vSize = vertices.size() * sizeof(glm::vec3);
		cSize = colors.size() * sizeof(glm::vec4);

		glGenBuffers(1, &v_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, v_VBO);
		glBufferData(GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));
		//glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));
		//Loc 0 = vPosition;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);

		//Loc 1 = vColor
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
		glEnableVertexAttribArray (1);
	}

	if(mesh->HasTextureCoords(0))
	{
		float *texcoords = new float[mesh->mNumVertices * 2];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			texcoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texcoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &t_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 2 * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);

		delete texcoords;
	}

	if(mesh->HasNormals())
	{
		float *normals = new float[mesh->mNumVertices * 3];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1,&n_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, n_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		// Loc 4 = vNormal
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (4);

		//delete normals;
	}

	if(mesh->HasFaces())
	{
		GLuint *indices = new GLuint[mesh->mNumFaces * 3];

		for(int i = 0; i < mesh->mNumFaces; i++)
		{
			//indices.push_back(glm::vec3(mesh->mFaces[i].mIndices[0],mesh->mFaces[i].mIndices[1],mesh->mFaces[i].mIndices[2]));

			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &i_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (3);

		delete indices;
	}

	if(mesh->HasTangentsAndBitangents())
	{
		float *tangents = new float[mesh->mNumVertices * 3];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			tangents[i * 3] = mesh->mTangents[i].x;
			tangents[i * 3 + 1] = mesh->mTangents[i].y;
			tangents[i * 3 + 2] = mesh->mTangents[i].z;
		}

		glGenBuffers(1,&tb_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, tb_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), tangents, GL_STATIC_DRAW);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (5);

		delete tangents;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void CreateMesh::setTexture(const char* filename, GLuint shaderID)
{
	isTextured = true;
	gSampler = glGetUniformLocation(shaderID, "diffuse_map");
	glUniform1i(gSampler, 0);

	texture = new TextureLoader(GL_TEXTURE_2D, filename);

	if (!texture->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

void CreateMesh::setNormalTexture(const char* textureName, GLuint shaderID)
{
	nSampler = glGetUniformLocation(shaderID, "relief_map");
	glUniform1i(nSampler, 1);

	normalTexture = new TextureLoader(GL_TEXTURE_2D,textureName);

	if (!normalTexture->Load()) 
	{
		std::cout << "Unable to load normal texture" << std::endl;
	}

}

void CreateMesh::setTgaTexture(const char* textureName, GLuint shaderID)
{
	nSampler = glGetUniformLocation(shaderID, "relief_map");
	glUniform1i(nSampler, 1);

	normalTexture = new TextureLoader(GL_TEXTURE_2D,textureName);

	if (!normalTexture->LoadTGA()) 
	{
		std::cout << "Unable to load normal texture" << std::endl;
	}

}

void CreateMesh::setCubeMapTexture(const char* directory, GLuint shaderID)
{
	gSampler = glGetUniformLocation(shaderID, "gSampler");
	glUniform1i(gSampler, 0);

	cubeTexture = new TextureLoader(directory);

	if (!cubeTexture->LoadCubeMap()) 
	{
		std::cout << "Unable to load cube map" << std::endl;
	}
}

void CreateMesh::render()
{
	if(isTextured)
	{
		texture->Bind(GL_TEXTURE0);
	}
	else
	{
		texture->UnBind();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void CreateMesh::renderSkyBox()
{
	cubeTexture->Bind(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void CreateMesh::renderNormalMap(GLuint shaderID)
{
	glActiveTexture(GL_TEXTURE0);
	texture->Bind(GL_TEXTURE0);
	//gSampler = glGetUniformLocation(shaderID, "gSampler");
	glUniform1i(gSampler, 0);

	glActiveTexture(GL_TEXTURE1);
	normalTexture->Bind(GL_TEXTURE1);
	//nSampler = glGetUniformLocation(shaderID, "nSampler");
	glUniform1i(nSampler, 1);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

}
