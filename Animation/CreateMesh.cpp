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

void CreateMesh::LoadMesh(const char* filename)
{
	Assimp::Importer importer;
	aiMesh *mesh;

	const aiScene *scene = importer.ReadFile(filename, 	aiProcess_Triangulate
		| aiProcess_OptimizeGraph
		| aiProcess_OptimizeMeshes
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs);

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
		glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));
		//Loc 0 = vPosition;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);

		//Loc 1 = vColor
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
		glEnableVertexAttribArray (1);
	}

	if(mesh->HasTextureCoords(0))
	{
		//glm::vec2 t = glm::vec2();

		//float *texcoords = new float[mesh->mNumVertices * 2];

		glm::vec2 t = glm::vec2();

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			//texcoords[i * 2] = mesh->mTextureCoords[0][i].x;
			//texcoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
			t.x = mesh->mTextureCoords[0][i].x;
			t.y = mesh->mTextureCoords[0][i].y;
			texcoords.push_back(t);
		}

		glGenBuffers(1, &t_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 2 * sizeof(GLfloat), (const GLvoid*)&texcoords[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);

		//delete texcoords;
	}

	if(mesh->HasNormals())
	{
		//float *normals = new float[mesh->mNumVertices * 3];
		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			/*normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;*/
			normals.push_back(glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z));
		}

		glGenBuffers(1,&n_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, n_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), (const GLvoid*)&normals[0], GL_STATIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CreateMesh::LoadMesh(const char* filename, bool isNormalMap)
{
		Assimp::Importer importer;
	aiMesh *mesh;

	const aiScene *scene = importer.ReadFile(filename, 	aiProcess_Triangulate
		| aiProcess_OptimizeGraph
		| aiProcess_OptimizeMeshes
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs);

	if(!scene) 
	{
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	mesh = scene->mMeshes[0];
	numElements = mesh->mNumFaces * 3;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if(mesh->HasTextureCoords(0))
	{
		//glm::vec2 t = glm::vec2();

		//float *texcoords = new float[mesh->mNumVertices * 2];

		glm::vec2 t = glm::vec2();

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			//texcoords[i * 2] = mesh->mTextureCoords[0][i].x;
			//texcoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
			t.x = mesh->mTextureCoords[0][i].x;
			t.y = mesh->mTextureCoords[0][i].y;
			texcoords.push_back(t);
		}

		glGenBuffers(1, &t_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 2 * sizeof(GLfloat), (const GLvoid*)&texcoords[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);

		//delete texcoords;
	}

	if(mesh->HasNormals())
	{
		//float *normals = new float[mesh->mNumVertices * 3];
		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			/*normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;*/
			normals.push_back(glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z));
		}

		glGenBuffers(1,&n_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, n_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), (const GLvoid*)&normals[0], GL_STATIC_DRAW);

		// Loc 4 = vNormal
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (4);

		//delete normals;
	}

	GLuint *indices = new GLuint[mesh->mNumFaces * 3];
	std::vector<int> face;

	if(mesh->HasFaces())
	{
		

		for(int i = 0; i < mesh->mNumFaces; i++)
		{
			//indices.push_back(glm::vec3(mesh->mFaces[i].mIndices[0],mesh->mFaces[i].mIndices[1],mesh->mFaces[i].mIndices[2]));

			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			face.push_back(indices[i * 3]);
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			face.push_back(indices[i * 3 + 1]);
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			face.push_back(indices[i * 3 + 2]);
		}

		glGenBuffers(1, &i_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (3);

		//delete indices;
	}

	if (isNormalMap)
	{
		if(mesh->HasPositions())
		{
			for(int i = 0; i < mesh->mNumVertices; i++) 
			{
				vertices.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));			
				//colors.push_back(glm::vec4(0.85f,0.85f,0.85f,1));
			}

			generateTangents(vertices,normals,face,texcoords,tangents);

			vSize = vertices.size() * sizeof(glm::vec3);
			cSize = colors.size() * sizeof(glm::vec4);

			glGenBuffers(1, &v_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, v_VBO);
			glBufferData(GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW);
			glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));
			glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&tangents[0]));
			//Loc 0 = vPosition;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray (0);

			//Loc 1 = vColor
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
			glEnableVertexAttribArray (1);
		}
	}
	else
	{
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
			glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));
			//Loc 0 = vPosition;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray (0);

			//Loc 1 = vColor
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
			glEnableVertexAttribArray (1);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CreateMesh::generateTangents(const std::vector<glm::vec3> & points, const std::vector<glm::vec3> & normals, 
								  const std::vector<int> & faces, const std::vector<glm::vec2> & texCoords, 
								  std::vector<glm::vec4> & tangents)
{
	std::vector<glm::vec3> tan1Accum;
	std::vector<glm::vec3> tan2Accum;

	for( int i = 0; i < points.size(); i++ ) {
		tan1Accum.push_back(glm::vec3(0.0f));
		tan2Accum.push_back(glm::vec3(0.0f));
		tangents.push_back(glm::vec4(0.0f));
	}

	// Compute the tangent vector
	for( int i = 0; i < faces.size(); i += 3 )
	{
		const glm::vec3 &p1 = points[faces[i]];
		const glm::vec3 &p2 = points[faces[i+1]];
		const glm::vec3 &p3 = points[faces[i+2]];

		const glm::vec2 &tc1 = texCoords[faces[i]];
		const glm::vec2 &tc2 = texCoords[faces[i+1]];
		const glm::vec2 &tc3 = texCoords[faces[i+2]];

		glm::vec3 q1 = p2 - p1;
		glm::vec3 q2 = p3 - p1;
		float s1 = tc2.x - tc1.x, s2 = tc3.x - tc1.x;
		float t1 = tc2.y - tc1.y, t2 = tc3.y - tc1.y;
		float r = 1.0f / (s1 * t2 - s2 * t1);
		glm::vec3 tan1( (t2*q1.x - t1*q2.x) * r,
			(t2*q1.y - t1*q2.y) * r,
			(t2*q1.z - t1*q2.z) * r);
		glm::vec3 tan2( (s1*q2.x - s2*q1.x) * r,
			(s1*q2.y - s2*q1.y) * r,
			(s1*q2.z - s2*q1.z) * r);
		tan1Accum[faces[i]] += tan1;
		tan1Accum[faces[i+1]] += tan1;
		tan1Accum[faces[i+2]] += tan1;
		tan2Accum[faces[i]] += tan2;
		tan2Accum[faces[i+1]] += tan2;
		tan2Accum[faces[i+2]] += tan2;
	}

	for( int i = 0; i < points.size(); ++i )
	{
		const glm::vec3 &n = normals[i];
		glm::vec3 &t1 = tan1Accum[i];
		glm::vec3 &t2 = tan2Accum[i];

		// Gram-Schmidt orthogonalize
		tangents[i] = glm::vec4(glm::normalize( t1 - (glm::dot(n,t1) * n) ), 0.0f);
		// Store handedness in w
		tangents[i].w = (glm::dot( glm::cross(n,t1), t2 ) < 0.0f) ? -1.0f : 1.0f;
	}
	tan1Accum.clear();
	tan2Accum.clear();
}

void CreateMesh::setTexture(const char* filename, GLuint shaderID)
{
	isTextured = true;
	GLuint gSampler = glGetUniformLocation(shaderID, "gSampler");
	glUniform1i(gSampler, 0);

	texture = new TextureLoader(GL_TEXTURE_2D, filename);

	if (!texture->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

void CreateMesh::setTexture(const char* filename)
{
	texture = new TextureLoader(GL_TEXTURE_2D, filename);

	if (!texture->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

void CreateMesh::Render()
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

void CreateMesh::RenderSkyBox()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void CreateMesh::Render(GLenum id)
{
	if(isTextured)
	{
		texture->Bind(id);
	}
	else
	{
		texture->UnBind();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}
