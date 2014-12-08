#include "Lab3Model.h"


Lab3Model::Lab3Model(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_camera = new Camera(m_setup);
	point_count = 0;
	humanSkeleton = new HumanSkeleton(m_setup);
	plane = new Cylinder();
}


Lab3Model::~Lab3Model(void)
{
}

void Lab3Model::run(void)
{
	m_setup->setupGlfwGlew();

	initShaders();

	humanSkeleton->createHumanNode();

	humanSkeleton->drawHumanMesh(m_shader->GetProgramID());


	plane = new Cylinder(100, 50, 50, glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(0.2, 0.2, 0.2, 1.0), 2);
	plane->generateObjectBuffer(m_shader->GetProgramID());



// 	if(!load_mesh("peter.dae", &vao, &point_count, &bone_offset_mats, &bone_count))
// 	{
// 		printf("load_mesh function crashed !");
// 	}

	do{
		m_setup->preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_camera->computeMatricesFromInputs();

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");

		m_camera->handleMVP(modelLoc, viewLoc, projLoc);

		glm::mat4 planeMat4 = glm::translate(glm::mat4(1),glm::vec3(0,0,0));
		planeMat4 = glm::rotate(glm::mat4(1), 90.0f, glm::vec3(-1,0,0));
		plane->update(planeMat4, m_shader->GetProgramID());
		plane->draw();

		
		humanSkeleton->updateStar(m_shader->GetProgramID());
		humanSkeleton->keyControl();
		//humanSkeleton->calcGlobalTransformation();
		humanSkeleton->updateHumanMesh(m_shader->GetProgramID());

// 		glUseProgram(m_shader->GetProgramID());
// 		glBindVertexArray(vao);
// 		glDrawArrays(GL_TRIANGLES, 0, point_count);

		// Swap buffers
		glfwSwapBuffers(m_setup->getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(m_setup->getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(m_setup->getWindow()) == 0 );
}


void Lab3Model::initShaders()
{
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("diffuse.vs",vertexShaderSourceCode);
	m_shader->readShaderFile("diffuse.ps",fragmentShaderSourceCode);
	GLuint vertexShaderID = m_shader->makeShader(vertexShaderSourceCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = m_shader->makeShader(fragmentShaderSourceCode.c_str(), GL_FRAGMENT_SHADER);
	m_shader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",m_shader->GetProgramID());
}


bool Lab3Model::load_mesh(std::string file_name, GLuint* vao, int* point_count, glm::mat4* bone_offset_mats, int* bone_count)
{
	const aiScene* scene = importer.ReadFile (file_name, aiProcess_Triangulate);

	if (!scene) 
	{
		std::cout << "ERROR: reading mesh - " << file_name << std::endl;
		return false;
	}

	std::cout << "Number of animations: " << scene->mNumAnimations << std::endl;
	std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;
	std::cout << "Number of cameras: " << scene->mNumCameras << std::endl;
	std::cout << "Number of lights: " << scene->mNumLights << std::endl;
	std::cout << "Number of materials: " << scene->mNumMaterials << std::endl;
	std::cout << "Number of textures: " << scene->mNumTextures << std::endl;

	/* get first mesh in file only */
	const aiMesh* mesh = scene->mMeshes[0];
	std::cout << "Number of vertices in mesh[0]: " << mesh->mNumVertices << std::endl;

	/* pass back number of vertex points in mesh */
	*point_count = mesh->mNumVertices;
	int pointNum = *point_count;

	glGenVertexArrays (1, vao);
	glBindVertexArray (*vao);
	
	/* we really need to copy out all the data from AssImp's funny little data
	structures into pure contiguous arrays before we copy it into data buffers
	because assimp's texture coordinates are not really contiguous in memory.
	i allocate some dynamic memory to do this. */

	GLfloat* points = NULL; // array of vertex points
	GLfloat* normals = NULL; // array of vertex normals
	GLfloat* texcoords = NULL; // array of texture coordinates
	GLint* bone_ids = NULL; // array of bone IDs
	if (mesh->HasPositions ()) 
	{
		points = new GLfloat[*point_count * 3 * sizeof (GLfloat)];

		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if (mesh->HasNormals ()) 
	{
		normals = new GLfloat[*point_count * 3 * sizeof (GLfloat)];

		for (int i = 0; i < *point_count; i++) 
		{
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) 
	{
		texcoords = new GLfloat[*point_count * 2 * sizeof (GLfloat)];

		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}

		/* extract bone weights */
	if (mesh->HasBones ()) {
		*bone_count = (int)mesh->mNumBones;

		int boneNum = *bone_count;
		/* an array of bones names. max 256 bones, max name length 64 */
		char bone_names[256][64];
		
		/* here I allocate an array of per-vertex bone IDs.
		each vertex must know which bone(s) affect it
		here I simplify, and assume that only one bone can affect each vertex,
		so my array is only one-dimensional
		*/
		bone_ids = (int*)malloc (pointNum * sizeof(int));

		std::string bName;
		int num_weights;

		
		for (int b_i = 0; b_i < boneNum; b_i++) 
		{
			const aiBone* bone = mesh->mBones[b_i];

			bName = mesh->mBones[b_i]->mName.data;
			num_weights =  mesh->mBones[b_i]->mNumWeights;
			
			/* get bone names */
			strcpy_s (bone_names[b_i], bone->mName.data);

			std::cout << "Bone_name[ " << b_i << "] = " << bName << std::endl;
			
			/* get [inverse] offset matrix for each bone */
			bone_offset_mats[b_i] = convertAssimpMatrix (bone->mOffsetMatrix);
			
			/* get bone weights
			we can just assume weight is always 1.0, because we are just using 1 bone
			per vertex. but any bone that affects a vertex will be assigned as the
			vertex' bone_id */
			for (int w_i = 0; w_i < num_weights; w_i++) 
			{
				aiVertexWeight weight = bone->mWeights[w_i];
				int vertex_id = (int)weight.mVertexId;

				// ignore weight if less than 0.5 factor
				if (weight.mWeight >= 0.5f) 
				{
					bone_ids[vertex_id] = b_i;
				}
			}
			
		} // endfor
		*bone_count = (int)mesh->mNumBones;
		*point_count = pointNum;
	} // endif


	/* copy mesh data into VBOs */
	if (mesh->HasPositions ()) 
	{
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * *point_count * sizeof (GLfloat),
			points,
			GL_STATIC_DRAW
			);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		delete [] points;
	}

	if (mesh->HasNormals ()) 
	{
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * *point_count * sizeof (GLfloat),
			normals,
			GL_STATIC_DRAW
			);
		glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (1);
		delete [] normals;
	}
	if (mesh->HasTextureCoords (0)) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			2 * *point_count * sizeof (GLfloat),
			texcoords,
			GL_STATIC_DRAW
			);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);
		delete [] texcoords;
	}
	if (mesh->HasBones ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			*point_count * sizeof (GLint),
			bone_ids,
			GL_STATIC_DRAW
			);
		glVertexAttribIPointer(3, 1, GL_INT, 0, NULL);
		glEnableVertexAttribArray (3);
		free (bone_ids);
	}

	
	std::cout << "Mesh Loaded" << std::endl;

	return true;
}

glm::mat4 Lab3Model::convertAssimpMatrix (aiMatrix4x4 m) 
{
	return glm::mat4 (
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4
		);
}