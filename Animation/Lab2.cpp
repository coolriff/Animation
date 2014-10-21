#include "Lab2.h"


Lab2::Lab2(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_camera = new Camera(m_setup);
	cylinder_1 = new Cylinder();
	cylinder_2 = new Cylinder();
	cylinder_3 = new Cylinder();
	cylinder_4 = new Cylinder();
	cylinder_5 = new Cylinder();
	cylinder_6 = new Cylinder();
	cylinder_7 = new Cylinder();
	cylinder_8 = new Cylinder();
	cylinder_9 = new Cylinder();
	cylinder_10 = new Cylinder();
	cylinder_11 = new Cylinder();
	cylinder_12 = new Cylinder();
	cylinder_13 = new Cylinder();
	cylinder_14 = new Cylinder();
	cylinder_15 = new Cylinder();
	cylinder_16 = new Cylinder();
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

	glm::vec4 cy_color_up = glm::vec4(1.0, 0.1, 0.1, 1.0);
	glm::vec4 cy_color_down = glm::vec4(0.1, 0.1, 1.0, 1.0);

	//root
	cylinder_1->generateVertices(glm::vec4(0, 0, 0, 0), 8.0, 4.0, 4.0, cy_color_up, cy_color_down,2);
	cylinder_1->generateObjectBuffer(m_shader->GetProgramID());

	// no 5 
	cylinder_2->generateVertices(glm::vec4(3.2, 8, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_2->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_3->generateVertices(glm::vec4(3.2, 11, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_3->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_4->generateVertices(glm::vec4(3.2, 14, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_4->generateObjectBuffer(m_shader->GetProgramID());

	// no 4
 	cylinder_5->generateVertices(glm::vec4(1.1, 8, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
  	cylinder_5->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_6->generateVertices(glm::vec4(1.1, 11, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_6->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_7->generateVertices(glm::vec4(1.1, 14, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_7->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_8->generateVertices(glm::vec4(-1.1, 8, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_8->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_9->generateVertices(glm::vec4(-1.1, 11, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_9->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_10->generateVertices(glm::vec4(-1.1, 14, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_10->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_11->generateVertices(glm::vec4(-3.2, 8, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_11->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_12->generateVertices(glm::vec4(-3.2, 11, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_12->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_13->generateVertices(glm::vec4(-3.2, 14, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_13->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_14->generateVertices(glm::vec4(-5, 4, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_14->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_15->generateVertices(glm::vec4(-5, 7, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_15->generateObjectBuffer(m_shader->GetProgramID());

	cylinder_16->generateVertices(glm::vec4(-5, 10, 0, 0), 3, 0.8, 0.8, cy_color_up, cy_color_down,16);
	cylinder_16->generateObjectBuffer(m_shader->GetProgramID());


	

// 	m_objectBuffer->GenerateVBO(vertices,colors);
// 	m_objectBuffer->LinkBufferToShader(m_shader->GetProgramID());

	do{
		m_setup->preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_camera->computeMatricesFromInputs();

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "MVP");

		m_camera->handleMVP(modelLoc);

		cylinder_1->draw();
		cylinder_2->draw();
		cylinder_3->draw();
		cylinder_4->draw();
		cylinder_5->draw();
		cylinder_6->draw();
		cylinder_7->draw();
		cylinder_8->draw();
		cylinder_9->draw();
		cylinder_10->draw();
		cylinder_11->draw();
		cylinder_12->draw();
		cylinder_13->draw();
		cylinder_14->draw();
		cylinder_15->draw();
		cylinder_16->draw();

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

void Lab2::createBones()
{

}
