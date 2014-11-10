#include "Lab3.h"


Lab3::Lab3(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_camera = new Camera(m_setup);
	bone = new Bone();
	armSkeleton = new ArmSkeleton(m_setup);
	cylinder = new Cylinder();
}


Lab3::~Lab3(void)
{
}

void Lab3::run(void)
{
	m_setup->setupGlfwGlew();

	initShaders();

	armSkeleton->createArmNode();

	armSkeleton->drawArmMesh(m_shader->GetProgramID());

	// 	m_objectBuffer->GenerateVBO(vertices,colors);
	// 	m_objectBuffer->LinkBufferToShader(m_shader->GetProgramID());

	do{
		m_setup->preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_camera->computeMatricesFromInputs();

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");

		m_camera->handleMVP(modelLoc, viewLoc, projLoc);

		
		armSkeleton->updateArmTarget(m_shader->GetProgramID());
 		armSkeleton->calculateInverseKinematics();
 		armSkeleton->updateArmMesh(m_shader->GetProgramID());
// 		if (armSkeleton->calculateInverseKinematics())
// 		{
// 			armSkeleton->updateArmMesh(m_shader->GetProgramID());
// 		}

		

		// Swap buffers
		glfwSwapBuffers(m_setup->getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(m_setup->getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(m_setup->getWindow()) == 0 );
}

void Lab3::initShaders()
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
