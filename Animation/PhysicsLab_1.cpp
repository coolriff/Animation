#include "PhysicsLab_1.h"


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
#define M_PI 3.1415926535897932384626433832795f

// CPU representation of a particle
struct Particles
{

	glm::vec3 pos;
	glm::vec3 speed;
	glm::vec3 velocity;
	glm::vec3 force;

	float mass;

	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particles& that) const 
	{
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 1000000;
Particles ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle()
{

	for(int i=LastUsedParticle; i<MaxParticles; i++)
	{
		if (ParticlesContainer[i].life < 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<LastUsedParticle; i++)
	{
		if (ParticlesContainer[i].life < 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void SortParticles()
{
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

PhysicsLab_1::PhysicsLab_1(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_camera = new Camera(m_setup);
	elapsed = 10000;
	ySpeed = 5.0f;
	rx = ry = rz = 0.0f;
	blackhole = glm::vec3(0,0,0);
	gravity = glm::vec3(0.0f,-9.81f, 0.0f);
}


PhysicsLab_1::~PhysicsLab_1(void)
{
	m_setup->cleanUp();
}


void PhysicsLab_1::run(void)
{
	m_setup->setupGlfwGlew();

	initShaders();
	initTweakBar();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Vertex shader
	GLuint CameraRight_worldspace_ID  = glGetUniformLocation(m_shader->GetProgramID(), "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID  = glGetUniformLocation(m_shader->GetProgramID(), "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(m_shader->GetProgramID(), "VP");

	// fragment shader
	GLuint TextureID  = glGetUniformLocation(m_shader->GetProgramID(), "myTextureSampler");

	static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	static GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];

	for(int i=0; i<MaxParticles; i++)
	{
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	GLuint Texture = loadDDS("particle.DDS");

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = 
	{ 
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	double lastTime = glfwGetTime();
	
	maindir = glm::vec3(0.0f, 10.0f, 0.0f);
	spread = 1.5f;

	do{
		m_setup->preDraw();

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

//		m_camera->computeMatricesFromInputs();
// 		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
// 		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
// 		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");
// 		m_camera->handleMVP(modelLoc, viewLoc, projLoc);

		m_camera->computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = m_camera->getProjectionMatrix();
		glm::mat4 ViewMatrix = m_camera->getViewMatrix();

		// We will need the camera's position in order to sort the particles
		// w.r.t the camera's distance.
		// There should be a getCameraPosition() function in common/controls.cpp, 
		// but this works too.
		glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);

		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticles = (int)(delta*10000.0);
		if (newparticles > (int)(0.016f*10000.0))
		{
			newparticles = (int)(0.016f*10000.0);
		}

		for(int i=0; i<newparticles; i++)
		{
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 25.0f; // This particle will live 5 seconds.
			ParticlesContainer[particleIndex].pos = glm::vec3(0,5,-20.0f);


// 			for (size_t i = 0; i < newparticles; ++i)
// 			{
// 				double ang = glm::linearRand(0.0, M_PI*2.0);
// 				ParticlesContainer[particleIndex].pos = glm::vec3(0,5,-20.0f) + glm::vec3(1.5f*sin(ang), 0.0, 1.5f*cos(ang));
// 			}


// 			static double time = 0.0;
// 			time += delta;

// 

// 			ParticlesContainer[particleIndex].pos.x = 0.15f*sinf((float)time*2.5f);
// 			ParticlesContainer[particleIndex].pos.y = 0.15f*cosf((float)time*2.5f);
// 			ParticlesContainer[particleIndex].pos.z = 1*0.25f*cosf((float)time*2.5f);


			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			glm::vec3 randomdir = glm::vec3(
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f
				);

			timeKeyControl();

			ParticlesContainer[particleIndex].speed = maindir + randomdir*spread * glm::vec3(1.0f,1.0f,1.0f);

			//ParticlesContainer[particleIndex].speed = maindir + randomdir * glm::vec3(0.0f,10.45f,0.0f);


			// Very bad way to generate a random color
			ParticlesContainer[particleIndex].r = rand() % 256;
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].b = rand() % 256;
			//ParticlesContainer[particleIndex].a = 255;
			//ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

			//ParticlesContainer[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;
			ParticlesContainer[particleIndex].size = 0.1f;

		}

		// Simulate all particles
		ParticlesCount = 0;
		for(int i=0; i<MaxParticles; i++)
		{

			Particles& p = ParticlesContainer[i]; // shortcut

			if(p.life > 0.0f)
			{

				// Decrease life
				p.life -= delta;
				p.a = (p.life * 10.0f);
				if (p.life > 0.0f)
				{

					// Simulate simple physics : gravity only, no collisions
					
					//p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
					p.speed += timeKeyControlGravity(delta);

// 					glm::vec3 randomdir = glm::vec3(
// 						(rand()%2000 - 1000.0f)/1000.0f,
// 						(rand()%2000 - 1000.0f)/1000.0f,
// 						(rand()%2000 - 1000.0f)/1000.0f
// 						);

					p.speed = glm::rotate(p.speed, (float)(180 * delta), p.speed);

// 					float diss = calcDistance(p.pos, glm::vec3(0,10,0));
// 					p.pos = glm::vec3(0,10,0)/diss*10.0f; 

					p.pos += p.speed * (float)delta;



					timeKeyControlBlackhole(delta);
					float dis = calcDistance(p.pos, blackhole);
					if (dis < 10)
					{
						p.pos = slerp(p.pos, blackhole, delta);
					}
// 					if (dis < 0.5f)
// 					{
// 						p.life = 0.0f;
// 					}

					//p.pos = slerp(p.pos, timeKeyControlBlackhole(delta),0.1f);
					p.cameradistance = glm::length2( p.pos - CameraPosition );
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

// 					if (glm::dot((p.pos - glm::vec3(0, 0, 0)), glm::vec3(0, 1, 0))<0.0001 && glm::dot(glm::vec3(0, 1, 0), p.speed)<0.0001)
// 					{
// 						p.pos.y = 0;
// 						p.speed.y *= -0.75;
// 					} 

					if (p.pos.y < 0)
					{
						p.pos.y = 0;
						p.speed.y *= -0.75;
					} 



					// Fill the GPU buffer
					g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
					g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
					g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

					g_particule_position_size_data[4*ParticlesCount+3] = p.size;

					g_particule_color_data[4*ParticlesCount+0] = p.r;
					g_particule_color_data[4*ParticlesCount+1] = p.g;
					g_particule_color_data[4*ParticlesCount+2] = p.b;
					g_particule_color_data[4*ParticlesCount+3] = p.a;

				}
				else
				{
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}

		SortParticles();


		//printf("%d ",ParticlesCount);


		// Update the buffers that OpenGL uses for rendering.
		// There are much more sophisticated means to stream data from the CPU to the GPU, 
		// but this is outside the scope of this tutorial.
		// http://www.opengl.org/wiki/Buffer_Object_Streaming


		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use our shader
		glUseProgram(m_shader->GetProgramID());

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// Same as the billboards tutorial
		glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// These functions are specific to glDrawArrays*Instanced*.
		// The first parameter is the attribute buffer we're talking about.
		// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

		// Draw the particules !
		// This draws many times a small triangle_strip (which looks like a quad).
		// This is equivalent to :
		// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
		// but faster.
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		TwDraw();
		// Swap buffers
		glfwSwapBuffers(m_setup->getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(m_setup->getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(m_setup->getWindow()) == 0 );

	delete[] g_particule_position_size_data;

	// Cleanup VBO and shader
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteProgram(m_shader->GetProgramID());
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}


void PhysicsLab_1::initShaders()
{
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("Particle.vertexshader",vertexShaderSourceCode);
	m_shader->readShaderFile("Particle.fragmentshader",fragmentShaderSourceCode);
	GLuint vertexShaderID = m_shader->makeShader(vertexShaderSourceCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = m_shader->makeShader(fragmentShaderSourceCode.c_str(), GL_FRAGMENT_SHADER);
	m_shader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",m_shader->GetProgramID());
}



GLuint PhysicsLab_1::loadDDS(const char * imagepath)
{
	unsigned char header[124];

	FILE *fp; 

	/* try to open the file */ 
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}

	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}

	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 

		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureID;


}

void PhysicsLab_1::timeKeyControl()
{
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_LEFT ) == GLFW_PRESS){
		rz += 0.005;
		maindir = glm::rotate(maindir,rz,glm::vec3(0,0,1));
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_RIGHT ) == GLFW_PRESS){
		rz -= 0.005;
		maindir = glm::rotate(maindir,rz,glm::vec3(0,0,1));
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_UP ) == GLFW_PRESS){
		rx += 0.005;
		maindir = glm::rotate(maindir,rx,glm::vec3(1,0,0));
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_DOWN ) == GLFW_PRESS){
		rx -= 0.005;
		maindir = glm::rotate(maindir,rx,glm::vec3(1,0,0));
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_O ) == GLFW_PRESS){
		spread += 0.0001f;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_P ) == GLFW_PRESS){
		spread -= 0.0001f;
	}


}

glm::vec3 PhysicsLab_1::timeKeyControlGravity(float delta)
{
	//gravity
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_Y ) == GLFW_PRESS){
		gravity.x += 0.0001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_U ) == GLFW_PRESS){
		gravity.x -= 0.0001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_H ) == GLFW_PRESS){
		gravity.y += 0.0001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_J ) == GLFW_PRESS){
		gravity.y -= 0.0001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_N ) == GLFW_PRESS){
		gravity.z += 0.0001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_M ) == GLFW_PRESS){
		gravity.z -= 0.0001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_L ) == GLFW_PRESS){
		gravity = glm::vec3(0.0f,-9.81f, 0.0f);
	}

	return gravity * (float)delta * 0.5f;

}

void PhysicsLab_1::timeKeyControlBlackhole(float delta)
{
	//blackhole
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_R ) == GLFW_PRESS){
		blackhole.x += 0.000001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_T ) == GLFW_PRESS){
		blackhole.x -= 0.000001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_F ) == GLFW_PRESS){
		blackhole.y += 0.000001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_G ) == GLFW_PRESS){
		blackhole.y -= 0.000001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_V ) == GLFW_PRESS){
		blackhole.z += 0.000001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_B ) == GLFW_PRESS){
		blackhole.z -= 0.000001f;
	}
	else if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_K ) == GLFW_PRESS){
		blackhole = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

glm::vec3 PhysicsLab_1::slerp (glm::vec3 p1, glm::vec3 p2, float t)
{
	//(1 - t)p1 + tp2

	glm::vec3 mDest; 
	mDest.x = ((1 - t) * p1.x) + (t * p2.x);
	mDest.y = ((1 - t) * p1.y) + (t * p2.y);
	mDest.z = ((1 - t) * p1.z) + (t * p2.z);

	return mDest;
}

void PhysicsLab_1::initTweakBar()
{
 	TwInit(TW_OPENGL_CORE, NULL);
 	TwWindowSize(1024,768);
 
 	bar = TwNewBar("Simulation Info: ");
	TwAddVarRO(bar, "Particles Alive", TW_TYPE_INT32, &ParticlesCount, " label='Particles: '");

	TwAddVarRO(bar, "Spread", TW_TYPE_FLOAT, &spread, " label='Spread(O,P): '");


// 	TwAddVarRO(bar, "Fountain X", TW_TYPE_FLOAT, &ParticlesContainer.pos.x, "group ='Fountain' label='Fountain.X: '");
// 	TwAddVarRO(bar, "Fountain Y", TW_TYPE_FLOAT, &ParticlesContainer[particleIndex].pos.y, "group ='Fountain' label='Fountain.Y: '");
// 	TwAddVarRO(bar, "Fountain Z", TW_TYPE_FLOAT, &ParticlesContainer[particleIndex].pos.z, "group ='Fountain' label='Fountain.Z: '");

	TwAddVarRO(bar, "Gravity X", TW_TYPE_FLOAT, &gravity.x, "group ='Gravity' label='Gravity.X: '");
	TwAddVarRO(bar, "Gravity Y", TW_TYPE_FLOAT, &gravity.y, "group ='Gravity' label='Gravity.Y: '");
	TwAddVarRO(bar, "Gravity Z", TW_TYPE_FLOAT, &gravity.z, "group ='Gravity' label='Gravity.Z: '");

	TwAddVarRO(bar, "Point1 X", TW_TYPE_FLOAT, &blackhole.x, "group ='Blackhole' label='Blackhole.X:'");
	TwAddVarRO(bar, "Point1 Y", TW_TYPE_FLOAT, &blackhole.y, "group ='Blackhole' label='Blackhole.Y:'");
	TwAddVarRO(bar, "Point1 Z", TW_TYPE_FLOAT, &blackhole.z, "group ='Blackhole' label='Blackhole.Z:'");
 
//  	glfwSetMouseButtonCallback(m_setup->getWindow(),(GLFWmousebuttonfun)TwEventMouseButtonGLFW);
//  	glfwSetCursorPosCallback(m_setup->getWindow(),(GLFWcursorposfun)TwEventMousePosGLFW);
//  	glfwSetScrollCallback(m_setup->getWindow(),(GLFWscrollfun)TwEventMouseWheelGLFW);
//  	glfwSetKeyCallback(m_setup->getWindow(),(GLFWkeyfun)TwEventKeyGLFW);
//  	glfwSetCharCallback(m_setup->getWindow(),(GLFWcharfun)TwEventCharGLFW);
}


float PhysicsLab_1::calcDistance(glm::vec3 pos1, glm::vec3 pos2)
{
	float dis = glm::distance(pos1, pos2);
	return dis;
}