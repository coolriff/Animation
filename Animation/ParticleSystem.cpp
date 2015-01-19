#include "ParticleSystem.h"
#include "ContentLoader.h"

ParticleSystem::ParticleSystem(void)
{
	textureName = "p";
	m_shader = new Shader();
	textureID = 0;
}


ParticleSystem::~ParticleSystem(void)
{
}

bool ParticleSystem::Initialise()
{
	initShaders();
	programID = m_shader->GetProgramID();
	glUseProgram(programID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);	

// 	mID = glGetUniformLocation(programID,"m");
// 	vID = glGetUniformLocation(programID,"v");
// 	pID = glGetUniformLocation(programID,"p");

	glGenBuffers(1, &colourBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &sizeBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBufferID);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	textureSampler  = glGetUniformLocation(programID, "myTextureSampler");
	ContentLoader::loadTexture("Models\\p.png", &textureID);

	glUseProgram(0);
	return true;
}

void ParticleSystem::Update(float timeDelta)
{
	colours.clear();
	vertices.clear();
	sizes.clear();
	std::list<Particle>::iterator it = particles.begin();

	while (it != particles.end())
	{
		if (it->alive)
		{
			UpdateParticle(timeDelta, * it);
			if (it->alive)
			{
				colours.push_back(it->diffuse);
				vertices.push_back(it->position);
				sizes.push_back(it->size);
			}			
		}
		it ++;
	}
}

void ParticleSystem::Draw()
{
	if (vertices.size() > 0)
	{
		glUseProgram(programID);

		//Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);	// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(textureSampler, 0);

		glEnable(GL_BLEND);	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SPRITE);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Copy the data into the buffers for drawing

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), & vertices[0]);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);
		glVertexAttribPointer(
			1,                  // attribute
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		glBufferSubData(GL_ARRAY_BUFFER, 0, colours.size() * sizeof(glm::vec4), & colours[0]);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, sizeBufferID);
		glVertexAttribPointer(
			2,                  // attribute
			1,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizes.size() * sizeof(float), & sizes[0]);

		// The particles are already in world space, so we wont be using this anyway

// 		glUniformMatrix4fv(mID, 1, GL_FALSE, & world[0][0]);
// 		glUniformMatrix4fv(vID, 1, GL_FALSE, & Game::Instance()->camera->view[0][0]);
// 		glUniformMatrix4fv(pID, 1, GL_FALSE, & Game::Instance()->camera->projection[0][0]);

		glUniform1f(pointSizeID, 20.0f);

		glDrawArrays(GL_POINTS, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glUseProgram(0);

		colours.clear();
		vertices.clear();
	}
}

void ParticleSystem::initShaders()
{
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("Particles.vs",vertexShaderSourceCode);
	m_shader->readShaderFile("Particles.ps",fragmentShaderSourceCode);
	GLuint vertexShaderID = m_shader->makeShader(vertexShaderSourceCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = m_shader->makeShader(fragmentShaderSourceCode.c_str(), GL_FRAGMENT_SHADER);
	m_shader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",m_shader->GetProgramID());
}
