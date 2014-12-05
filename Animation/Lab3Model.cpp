#include "Lab3Model.h"
#include <Windows.h>


Lab3Model::Lab3Model(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_camera = new Camera(m_setup);
	point_count = 0;
	m_pEffect = new SkinningTechnique();
}


Lab3Model::~Lab3Model(void)
{
}

void Lab3Model::run(void)
{
	m_setup->setupGlfwGlew();

//	initShaders();

	if (!m_pEffect->Init()) {
		printf("Error initializing the lighting technique\n");
	}

	m_pEffect->Enable();

	m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);

	if (!m_mesh.LoadMesh("boblampclean.md5mesh")) {
		printf("Mesh load failed\n");          
	}

	do{
		m_setup->preDraw();

		m_pEffect->Enable();

		vector<Matrix4f> Transforms;

		float RunningTime = this->GetRunningTime();

		m_mesh.BoneTransform(RunningTime, Transforms);

// 		m_camera->computeMatricesFromInputs();
// 
// 		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
// 		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
// 		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");
// 
// 		m_camera->handleMVP(modelLoc, viewLoc, projLoc);

		for (uint i = 0 ; i < Transforms.size() ; i++) {
			m_pEffect->SetBoneTransform(i, Transforms[i]);
		}

// 		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
// 
// 		Pipeline p;
// 		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
// 		p.SetPerspectiveProj(m_persProjInfo);           
// 		p.Scale(0.1f, 0.1f, 0.1f);                
// 
// 		Vector3f Pos(m_position);
// 		p.WorldPos(Pos);        
// 		p.Rotate(270.0f, 180.0f, 0.0f);       
// 		m_pEffect->SetWVP(p.GetWVPTrans());
// 		m_pEffect->SetWorldMatrix(p.GetWorldTrans());     


		m_mesh.Render();

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

float Lab3Model::GetRunningTime()
{
	float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
	return RunningTime;
}

glm::mat4 Lab3Model::convertAssimpMatrix (aiMatrix4x4 m) 
{
	glm::mat4 to;

	to[0][0] = m.a1; to[0][1] = m.b1; to[0][2] = m.c1; to[0][3] = m.d1;
	to[1][0] = m.a2; to[1][1] = m.b2; to[1][2] = m.c2; to[1][3] = m.d2;
	to[2][0] = m.a3; to[2][1] = m.b3; to[2][2] = m.c3; to[2][3] = m.d3;
	to[3][0] = m.a4; to[3][1] = m.b4; to[3][2] = m.c4; to[3][3] = m.d4;

	return to;
}