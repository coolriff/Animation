#include "ObjectBuffer.h"


ObjectBuffer::ObjectBuffer(void)
{
}

ObjectBuffer::ObjectBuffer(GLuint numVertices)
{
	this->numVertices = numVertices;
}

ObjectBuffer::~ObjectBuffer(void)
{
}

GLuint ObjectBuffer::GenerateVBO(GLfloat vertices[], GLfloat colors[])
{
	// Create the "remember all"
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	// buffer will contain an array of vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but do not load anything yet
	glBufferData(GL_ARRAY_BUFFER, numVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	// load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices*3*sizeof(GLfloat), vertices);

	// Load the colors right after vertex
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), numVertices*4*sizeof(GLfloat), colors);

	return vbo;
}

GLuint ObjectBuffer::GenerateVBO(const std::vector<glm::vec3> & vertices, GLfloat colors[])
{
	// Create the "remember all"
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint vSize = vertices.size() * sizeof(glm::vec3);

	// buffer will contain an array of vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but do not load anything yet
	glBufferData(GL_ARRAY_BUFFER, numVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	// load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));

	// Load the colors right after vertex
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), numVertices*4*sizeof(GLfloat), colors);

	return vbo;
}


GLuint ObjectBuffer::GenerateVBO(const std::vector<glm::vec3> & vertices, const std::vector<glm::vec4> & colors , const std::vector<glm::vec3> & normals)
{
	// Create the "remember all"
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	vSize = vertices.size() * sizeof(glm::vec3);
	nSize = normals.size() * sizeof(glm::vec3);
	cSize = colors.size() * sizeof(glm::vec4);

	// buffer will contain an array of vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but do not load anything yet
	glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize, NULL, GL_STATIC_DRAW);

	// load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize, nSize, (const GLvoid*)(&normals[0]));

	return vbo;
}

GLuint ObjectBuffer::GenerateVBO(const std::vector<glm::vec3> & vertices, 
								 const std::vector<glm::vec4> & colors , 
								 const std::vector<glm::vec3> & normals, 
								 const std::vector<glm::vec2> & texcoords, 
								 const std::vector<glm::vec3> & indices)
{
	// Create the "remember all"
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	vSize = vertices.size() * sizeof(glm::vec3);
	nSize = normals.size() * sizeof(glm::vec3);
	cSize = colors.size() * sizeof(glm::vec4);
	tSize = texcoords.size() * sizeof(glm::vec2);
	iSize = indices.size() * sizeof(glm::vec3);

	std::cout << vertices.size() << std::endl;
	std::cout << normals.size() << std::endl;
	std::cout << texcoords.size() << std::endl;
	std::cout << colors.size() << std::endl;
	std::cout << indices.size() << std::endl;
	// buffer will contain an array of vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but do not load anything yet
	glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize + tSize + iSize, NULL, GL_STATIC_DRAW);

	// load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize, nSize, (const GLvoid*)(&normals[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize + nSize, tSize, (const GLvoid*)(&texcoords[0]));
	glBufferSubData(GL_ARRAY_BUFFER, vSize + cSize + nSize + tSize, iSize, (const GLvoid*)(&indices[0]));

	return vbo;
}

void ObjectBuffer::LinkBufferToShader(GLuint shaderProgramID)
{
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");

	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*3*sizeof(GLfloat)));

	//glBindVertexArray(0);
}

void ObjectBuffer::LinkBufferToShaderWithNormal(GLuint shaderProgramID)
{
	glBindVertexArray( vao );	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");
	GLuint normalID = glGetAttribLocation(shaderProgramID, "vNormal");

	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));

	glEnableVertexAttribArray(normalID);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize + cSize));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ObjectBuffer::ChangeColors(const std::vector<glm::vec4> &newColor)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&newColor[0]));
}

void ObjectBuffer::SetColor(glm::vec4 color)
{
	std::vector<glm::vec4> newColor; 

	for(int i = 0; i < cSize; i++)
	{
		newColor.push_back(glm::vec4(color));
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&newColor[0]));
}

void ObjectBuffer::SetTexture(const char* filename, GLuint shaderProgramID)
{
	gSampler = glGetUniformLocation(shaderProgramID, "gSampler");

	glUniform1i(gSampler, 0);

	textureLoader = new TextureLoader(GL_TEXTURE_2D, filename);

	if (!textureLoader->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

void ObjectBuffer::tony(const std::vector<glm::vec3> & vertices, 
						const std::vector<glm::vec4> & colors , 
						const std::vector<glm::vec3> & normals, 
						const std::vector<glm::vec2> & texcoords, 
						const std::vector<glm::vec3> & indices)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vSize = vertices.size() * sizeof(glm::vec3);
	cSize = vertices.size() * sizeof(glm::vec4);

	glGenBuffers(1, &vboV);
	glBindBuffer(GL_ARRAY_BUFFER, vboV);
	glBufferData(GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);

	// Just incase we use colors
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
	glEnableVertexAttribArray (1);

	// -------------------------------------------------------------------------------

	glGenBuffers(1, &vboT);
	glBindBuffer(GL_ARRAY_BUFFER,vboT);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2 * sizeof(GLfloat), (const GLvoid*)(&texcoords[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (2);





	glGenBuffers(1,&vboN);
	glBindBuffer(GL_ARRAY_BUFFER, vboN);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(GLfloat),(const GLvoid*)(&normals[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (4);



	glGenBuffers(1, &vboI);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboI);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vertices.size() /3) * sizeof(GLuint),(const GLvoid*)(&indices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (3);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
