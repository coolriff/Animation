#include "RenderingLab1n2.h"

//typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
{
	TwEventMouseButtonGLFW( button , action );
}

//typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
void MousePosCB(GLFWwindow*,double x ,double y)
{
	TwEventMousePosGLFW( (int)x, (int)y );
}

//typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
{
	TwEventKeyGLFW( key , action );
	TwEventCharGLFW( key  , action );
}

//typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
void MouseScrollCB(  GLFWwindow* window, double x , double y )
{
	TwEventMouseWheelGLFW( (int)y );
}


RenderingLab1n2::RenderingLab1n2(void)
{
	m_physicsLabCamera = new PhysicsLabCamera();
	stopTime = false;
	useForce = false;

	
	isTexture = false;

	shaderDefault = new Shader();
	shaderToon = new Shader();
	shaderBlinnPhong = new Shader();
	shaderOrenNayar = new Shader();

	for (int i=0; i<MAXOBJECT; i++)
	{
		m_body[i] = new Cube();
		m_bodyMesh[i] = new CreateMesh();
		m_bodyBuffer[i] = new ObjectBuffer();
		dShader[i] = false;
		tShader[i] = false;
		bShader[i] = false;
		oShader[i] = false;
		shaderType[i]= DEFAULT;
	}

	eye = m_physicsLabCamera->direction;

	ambientColor = glm::vec3(1.0f,1.0f,1.0f);
	ambientIntensity = 0.1f;	

	diffuseColor = glm::vec3(1.0f,1.0f,1.0f);
	diffuseIntensity = 0.1f;
	diffuseDirection = glm::vec3(0, 0, -1);

	specularColor = glm::vec3(1.0f,1.0f,1.0f);
	specularIntensity = 0.7f;
	specularShininess = 60.0f;

	roughness = 1.0f;
}


RenderingLab1n2::~RenderingLab1n2(void)
{
	cleanUp();
	TwTerminate();
}


void RenderingLab1n2::run(void)
{
	setupGlfwGlew();
	initShaders();
	initTweakBar();

	double lastTime = glfwGetTime();

	for (int i=0; i<MAXOBJECT; i++)
	{
		m_bodyMesh[i]->LoadMesh("../Models/teapot2.obj");
		m_bodyBuffer[i]->GenerateVBO(m_bodyMesh[i]->vertices, m_bodyMesh[i]->colors, m_bodyMesh[i]->normals);
		m_bodyBuffer[i]->LinkBufferToShaderWithNormal(shaderDefault->GetProgramID());
		m_bodyBuffer[i]->LinkBufferToShaderWithNormal(shaderToon->GetProgramID());
		m_bodyBuffer[i]->LinkBufferToShaderWithNormal(shaderBlinnPhong->GetProgramID());
		m_bodyBuffer[i]->LinkBufferToShaderWithNormal(shaderOrenNayar->GetProgramID());
	}
	m_body[0]->SetPosition(glm::vec3(-5,0,0));
	m_body[1]->SetPosition(glm::vec3(5,0,0));
	

	do{
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		if (stopTime)
		{
			delta = 0;
		}

		preDraw();
		
// 		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET ) == GLFW_PRESS){
// 			isTexture = true;
// 		}
// 
// 		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET ) == GLFW_PRESS){
// 			isTexture = false;
// 		}



		for (int i=0; i<MAXOBJECT; i++)
		{
			m_body[i]->Update(delta);
		}

		keyControl();

		for (int i=0; i<MAXOBJECT; i++)
		{
			switch (shaderType[i])
			{
			case RenderingLab1n2::DEFAULT:
				glUseProgram(shaderDefault->GetProgramID());
				m_physicsLabCamera->computeMatricesFromInputs(window);
				modelLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "model");
				viewLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "view");
				projLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "projection");
				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
				shaderDefault->findAllShaderID();
				shaderDefault->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);
				shaderDefault->SetAmbientLight(ambientColor,ambientIntensity);
				shaderDefault->SetSpecularComponent(specularColor,specularIntensity,specularShininess);
				update(m_body[i]->GetTransformationMatrix(), shaderDefault->GetProgramID());
				draw(m_bodyBuffer[i]->vao, m_bodyMesh[i]->vertices.size()); 
 				break;
			case RenderingLab1n2::TOON:
				glUseProgram(shaderToon->GetProgramID());
				m_physicsLabCamera->computeMatricesFromInputs(window);
				modelLoc = glGetUniformLocation(shaderToon->GetProgramID(), "model");
				viewLoc = glGetUniformLocation(shaderToon->GetProgramID(), "view");
				projLoc = glGetUniformLocation(shaderToon->GetProgramID(), "projection");
				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
				shaderToon->findAllShaderID();
				shaderToon->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);
				shaderToon->SetAmbientLight(ambientColor,ambientIntensity);
				shaderToon->SetSpecularComponent(specularColor,specularIntensity,specularShininess);
				update(m_body[i]->GetTransformationMatrix(), shaderToon->GetProgramID());
				draw(m_bodyBuffer[i]->vao, m_bodyMesh[i]->vertices.size()); 
				break;
			case RenderingLab1n2::BLINNPHONG:
				glUseProgram(shaderBlinnPhong->GetProgramID());
				m_physicsLabCamera->computeMatricesFromInputs(window);
				modelLoc = glGetUniformLocation(shaderBlinnPhong->GetProgramID(), "model");
				viewLoc = glGetUniformLocation(shaderBlinnPhong->GetProgramID(), "view");
				projLoc = glGetUniformLocation(shaderBlinnPhong->GetProgramID(), "projection");
				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
				shaderBlinnPhong->findAllShaderID();
				shaderBlinnPhong->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);
				shaderBlinnPhong->SetAmbientLight(ambientColor,ambientIntensity);
				shaderBlinnPhong->SetSpecularComponent(specularColor,specularIntensity,specularShininess);
				update(m_body[i]->GetTransformationMatrix(), shaderBlinnPhong->GetProgramID());
				draw(m_bodyBuffer[i]->vao, m_bodyMesh[i]->vertices.size()); 
				break;
			case RenderingLab1n2::OREN_NAYAR:
				glUseProgram(shaderOrenNayar->GetProgramID());
				m_physicsLabCamera->computeMatricesFromInputs(window);
				modelLoc = glGetUniformLocation(shaderOrenNayar->GetProgramID(), "model");
				viewLoc = glGetUniformLocation(shaderOrenNayar->GetProgramID(), "view");
				projLoc = glGetUniformLocation(shaderOrenNayar->GetProgramID(), "projection");
				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
				shaderOrenNayar->findAllShaderID();
				shaderOrenNayar->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);
				shaderOrenNayar->SetAmbientLight(ambientColor,ambientIntensity);
				shaderOrenNayar->SetSpecularComponent(specularColor,specularIntensity,specularShininess);
				//shaderOrenNayar->SetEyeVector(m_physicsLabCamera->position + m_physicsLabCamera->direction);
				update(m_body[i]->GetTransformationMatrix(), shaderOrenNayar->GetProgramID());
				draw(m_bodyBuffer[i]->vao, m_bodyMesh[i]->vertices.size()); 
				break;
			}
		}

		TwDraw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );

	TwTerminate();
	glfwTerminate();
}

void RenderingLab1n2::initShaders()
{
	std::string v1,f1;
	shaderDefault->readShaderFile("default.vs",v1);
	shaderDefault->readShaderFile("default.ps",f1);
	GLuint vertexShaderID_T1 = shaderDefault->makeShader(v1.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_T1 = shaderDefault->makeShader(f1.c_str(), GL_FRAGMENT_SHADER);
	shaderDefault->makeShaderProgram(vertexShaderID_T1,fragmentShaderID_T1);
	printf("vertexShaderID is %d\n",vertexShaderID_T1);
	printf("fragmentShaderID is %d\n",fragmentShaderID_T1);
	printf("shaderProgramID is %d\n",shaderDefault->GetProgramID());

	std::string v2,f2;
	shaderToon->readShaderFile("too.vs",v2);
	shaderToon->readShaderFile("too.ps",f2);
	GLuint vertexShaderID_T2 = shaderToon->makeShader(v2.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_T2 = shaderToon->makeShader(f2.c_str(), GL_FRAGMENT_SHADER);
	shaderToon->makeShaderProgram(vertexShaderID_T2,fragmentShaderID_T2);
	printf("vertexShaderID is %d\n",vertexShaderID_T2);
	printf("fragmentShaderID is %d\n",fragmentShaderID_T2);
	printf("shaderProgramID is %d\n",shaderToon->GetProgramID());

	std::string v3,f3;
	shaderBlinnPhong->readShaderFile("BlinnPhong.vs",v3);
	shaderBlinnPhong->readShaderFile("BlinnPhong.ps",f3);
	GLuint vertexShaderID_T3 = shaderBlinnPhong->makeShader(v3.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_T3 = shaderBlinnPhong->makeShader(f3.c_str(), GL_FRAGMENT_SHADER);
	shaderBlinnPhong->makeShaderProgram(vertexShaderID_T3,fragmentShaderID_T3);
	printf("vertexShaderID is %d\n",vertexShaderID_T3);
	printf("fragmentShaderID is %d\n",fragmentShaderID_T3);
	printf("shaderProgramID is %d\n",shaderBlinnPhong->GetProgramID());

	std::string v4,f4;
	shaderOrenNayar->readShaderFile("orenNayar.vs",v4);
	shaderOrenNayar->readShaderFile("orenNayar.ps",f4);
	GLuint vertexShaderID_T4 = shaderOrenNayar->makeShader(v4.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_T4 = shaderOrenNayar->makeShader(f4.c_str(), GL_FRAGMENT_SHADER);
	shaderOrenNayar->makeShaderProgram(vertexShaderID_T4,fragmentShaderID_T4);
	printf("vertexShaderID is %d\n",vertexShaderID_T4);
	printf("fragmentShaderID is %d\n",fragmentShaderID_T4);
	printf("shaderProgramID is %d\n",shaderOrenNayar->GetProgramID());
}

void RenderingLab1n2::setupGlfwGlew()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HIGH, "Lab 02", NULL, NULL);

	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);


	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(WINDOW_WIDTH,WINDOW_HIGH);

	/* Set GLFW event callbacks */
	// - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetMouseButtonCallback( window , MouseButtonCB );
	// - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetCursorPosCallback( window , MousePosCB);
	// - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetScrollCallback( window , MouseScrollCB );
	// - Directly redirect GLFW key events to AntTweakBar
	glfwSetKeyCallback(window , KeyFunCB);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// 	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HIGH/2);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glClearColor(0.0f, 0.2f, 0.2f, 0.0f);

	// 	// Cull triangles which normal is not towards the camera
	// 	glEnable(GL_CULL_FACE);
}

void RenderingLab1n2::cleanUp()
{
	glfwTerminate();
}

void RenderingLab1n2::preDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* RenderingLab1n2::getWindow()
{
	return window;
}

void RenderingLab1n2::initTweakBar()
{
	TwBar * bar;
	bar = TwNewBar("Simulation");
	TwDefine(" Simulation size='300 400' ");


	TwAddVarRW(bar, "def", TW_TYPE_BOOL8, &dShader, " label='Default(7) '");
	TwAddVarRW(bar, "Too", TW_TYPE_BOOL8, &tShader, " label='Toon(8) '");
	TwAddVarRW(bar, "bp", TW_TYPE_BOOL8, &bShader, " label='BlinnPhong(9) '");
	TwAddVarRW(bar, "on", TW_TYPE_BOOL8, &oShader, " label='Oren Nayar(0) '");

	TwAddVarRW(bar, "ambientColor", TW_TYPE_COLOR3F, &ambientColor, " label='AmbientColor '");
	TwAddVarRW(bar, "ambientIntensity", TW_TYPE_FLOAT, &ambientIntensity, " label='AmbientIntensity '");

	TwAddVarRW(bar, "diffuseColor", TW_TYPE_COLOR3F, &diffuseColor, " label='DiffuseColor '");
	TwAddVarRW(bar, "diffuseIntensity", TW_TYPE_FLOAT, &diffuseIntensity, " label='DiffuseIntensity '");
	TwAddVarRW(bar, "diffuseDirection", TW_TYPE_DIR3F, &diffuseDirection, " label='DiffuseDirection '");

	TwAddVarRW(bar, "specularColor", TW_TYPE_COLOR3F, &specularColor, " label='SpecularColor '");
	TwAddVarRW(bar, "specularIntensity", TW_TYPE_FLOAT, &specularIntensity, " label='SpecularIntensity '");
	TwAddVarRW(bar, "specularShininess", TW_TYPE_FLOAT, &specularShininess, " label='SpecularShininess '");
	TwAddVarRW(bar, "roughness", TW_TYPE_FLOAT, &roughness, " label='Roughness '");

}

void RenderingLab1n2::update(glm::mat4 ModelMatrix, GLuint shaderProgramID)
{
	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
}

void RenderingLab1n2::keyControl()
{
	if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
		shaderType[0] = DEFAULT;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
		shaderType[0] = TOON;
	}
	if (glfwGetKey(window, GLFW_KEY_Y ) == GLFW_PRESS){
		shaderType[0] = BLINNPHONG;
	}
	if (glfwGetKey(window, GLFW_KEY_U ) == GLFW_PRESS){
		shaderType[0] = OREN_NAYAR;
	}


	if (glfwGetKey(window, GLFW_KEY_F ) == GLFW_PRESS){
		shaderType[1] = DEFAULT;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
		shaderType[1] = TOON;
	}
	if (glfwGetKey(window, GLFW_KEY_H ) == GLFW_PRESS){
		shaderType[1] = BLINNPHONG;
	}
	if (glfwGetKey(window, GLFW_KEY_J ) == GLFW_PRESS){
		shaderType[1] = OREN_NAYAR;
	}

	//transfer
	if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			m_body[i]->SetAngularMomentum(glm::vec3(0.5f,0,0));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			m_body[i]->SetAngularMomentum(glm::vec3(0,0.5f,0));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			m_body[i]->SetAngularMomentum(glm::vec3(0,0,0.5f));
		}
	}

	//dt
	if (glfwGetKey(window, GLFW_KEY_P ) == GLFW_PRESS){
		stopTime = true;
	}

	if (glfwGetKey(window, GLFW_KEY_O ) == GLFW_PRESS){
		stopTime = false;
	}
}

void RenderingLab1n2::draw(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, size);
	glBindVertexArray(0);
}

void RenderingLab1n2::drawLine(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, size);
	glBindVertexArray(0);
}