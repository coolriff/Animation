#include "Lab2.h"


Lab2::Lab2(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_camera = new Camera(m_setup);
	bone = new Bone();
}


Lab2::~Lab2(void)
{
	m_setup->cleanUp();
}


void Lab2::run(void)
{
	m_setup->setupGlfwGlew();

	initShaders();



	createHandNode();

	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	for(int i = 0; i < 16; i++) 
	{
		if (i == 0)
		{
			cylinder[i] = new Cylinder(glm::vec4(bone->getPos(handNode[i]),0), 8.0, 4.0, 4.0, cy_color_up, cy_color_down,2);
			cylinder[i]->generateObjectBuffer(m_shader->GetProgramID());
		}
		else
		{
			cylinder[i] = new Cylinder(glm::vec4(bone->getPos(handNode[i]),0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
			cylinder[i]->generateObjectBuffer(m_shader->GetProgramID());
		}
	}

// 	m_objectBuffer->GenerateVBO(vertices,colors);
// 	m_objectBuffer->LinkBufferToShader(m_shader->GetProgramID());

	do{
		m_setup->preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_camera->computeMatricesFromInputs();

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "MVP");

		m_camera->handleMVP(modelLoc);

		for (int i = 0; i < 16; i++)
		{
			cylinder[i]->draw();
		}

// 		glBindVertexArray(cylinder->vertex_array_object_id);
// 		glDrawArrays(GL_TRIANGLES, 0, 12*3);
// 		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(m_setup->getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(m_setup->getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(m_setup->getWindow()) == 0 );
}


void Lab2::initShaders()
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

void Lab2::createHandNode()
{
	//root
	handNode[0] = bone->createBone(0,glm::vec3(0,0,0));

	//no 5
	handNode[1] = bone->createBone(1,glm::vec3(3.2,8,0));
	handNode[2] = bone->createBone(2,glm::vec3(3.2,11,0));
	handNode[3] = bone->createBone(3,glm::vec3(3.2,14,0));

	//no 4
	handNode[4] = bone->createBone(4,glm::vec3(1.1,8,0));
	handNode[5] = bone->createBone(5,glm::vec3(1.1,11,0));
	handNode[6] = bone->createBone(6,glm::vec3(1.1,14,0));

	//no 3
	handNode[7] = bone->createBone(7,glm::vec3(-1.1,8,0));
	handNode[8] = bone->createBone(8,glm::vec3(-1.1,11,0));
	handNode[9] = bone->createBone(9,glm::vec3(-1.1,14,0));

	//no 2
	handNode[10] = bone->createBone(10,glm::vec3(-3.2,8,0));
	handNode[11] = bone->createBone(11,glm::vec3(-3.2,11,0));
	handNode[12] = bone->createBone(12,glm::vec3(-3.2,14,0));

	//no 1
	handNode[13] = bone->createBone(13,glm::vec3(-5,4,0));
	handNode[14] = bone->createBone(14,glm::vec3(-5,7,0));
	handNode[15] = bone->createBone(15,glm::vec3(-5,10,0));
}
