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
	dShader = false;
	tShader = false;
	bShader = false;
	oShader = false;
	shaderType = DEFAULT;
	isTexture = false;

	shaderDefault = new Shader();
	shaderToon = new Shader();
	shaderBlinnPhong = new Shader();
	shaderOrenNayar = new Shader();

	m_body = new Cube();
	m_bodyMesh = new CreateMesh();
	m_bodyBuffer = new ObjectBuffer();


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

	m_body->SetPosition(glm::vec3(0));
	m_bodyMesh->LoadMesh("../Models/teapot2.obj");
	m_bodyBuffer->GenerateVBO(m_bodyMesh->vertices, m_bodyMesh->colors, m_bodyMesh->normals);
	m_bodyBuffer->LinkBufferToShaderWithNormal(shaderDefault->GetProgramID());
	m_bodyBuffer->LinkBufferToShaderWithNormal(shaderToon->GetProgramID());
	m_bodyBuffer->LinkBufferToShaderWithNormal(shaderBlinnPhong->GetProgramID());
	m_bodyBuffer->LinkBufferToShaderWithNormal(shaderOrenNayar->GetProgramID());

// 	shaderDefault->findAllShaderID();
// 	shaderToon->findAllShaderID();
// 	shaderBlinnPhong->findAllShaderID();
// 	shaderOrenNayar->findAllShaderID();

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

		if (glfwGetKey(window, GLFW_KEY_7 ) == GLFW_PRESS){
			shaderType = DEFAULT;
			dShader = true;
			tShader = false;
			bShader = false;
			oShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
			shaderType = TOON;
			dShader = false;
			tShader = true;
			bShader = false;
			oShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_9 ) == GLFW_PRESS){
			shaderType = BLINNPHONG;
			dShader = false;
			tShader = false;
			bShader = true;
			oShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_0 ) == GLFW_PRESS){
			shaderType = OREN_NAYAR;
			dShader = false;
			tShader = false;
			bShader = false;
			oShader = true;


		}


		switch (shaderType)
		{
		case RenderingLab1n2::DEFAULT:
			glUseProgram(shaderDefault->GetProgramID());
 			break;
		case RenderingLab1n2::TOON:
			glUseProgram(shaderToon->GetProgramID());
			break;
		case RenderingLab1n2::BLINNPHONG:
			glUseProgram(shaderBlinnPhong->GetProgramID());
			break;
		case RenderingLab1n2::OREN_NAYAR:
			glUseProgram(shaderOrenNayar->GetProgramID());
			break;
		}

		keyControl();
		
		if (shaderType == DEFAULT)
		{
			m_body->Update(delta);

			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			shaderDefault->findAllShaderID();
			shaderDefault->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);

			update(m_body->GetTransformationMatrix(), shaderDefault->GetProgramID());
			draw(m_bodyBuffer->vao, m_bodyMesh->vertices.size()); 

		}

		if (shaderType == TOON)
		{
			m_body->Update(delta);

			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(shaderToon->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(shaderToon->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(shaderToon->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			shaderToon->findAllShaderID();
			shaderToon->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);

			update(m_body->GetTransformationMatrix(), shaderToon->GetProgramID());
			draw(m_bodyBuffer->vao, m_bodyMesh->vertices.size()); 
		}

		if (shaderType == BLINNPHONG)
		{
			m_body->Update(delta);

			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(shaderBlinnPhong->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(shaderBlinnPhong->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(shaderBlinnPhong->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			shaderBlinnPhong->findAllShaderID();
			shaderBlinnPhong->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);

			update(m_body->GetTransformationMatrix(), shaderBlinnPhong->GetProgramID());
			draw(m_bodyBuffer->vao, m_bodyMesh->vertices.size()); 
		}

		if (shaderType == OREN_NAYAR)
		{
			m_body->Update(delta);

			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(shaderOrenNayar->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(shaderOrenNayar->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(shaderOrenNayar->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);


			shaderOrenNayar->findAllShaderID();
			shaderOrenNayar->SetDirectionalLight(diffuseDirection,diffuseColor,diffuseIntensity);
			shaderOrenNayar->SetAmbientLight(ambientColor,ambientIntensity);
			shaderOrenNayar->SetSpecularComponent(specularColor,specularIntensity,specularShininess);
			shaderOrenNayar->SetEyeVector(m_physicsLabCamera->position + m_physicsLabCamera->direction);

			update(m_body->GetTransformationMatrix(), shaderOrenNayar->GetProgramID());
			draw(m_bodyBuffer->vao, m_bodyMesh->vertices.size()); 
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


	TwAddVarRO(bar, "def", TW_TYPE_BOOL8, &dShader, " label='Default(7) '");
	TwAddVarRO(bar, "Too", TW_TYPE_BOOL8, &tShader, " label='Toon(8) '");
	TwAddVarRO(bar, "bp", TW_TYPE_BOOL8, &bShader, " label='BlinnPhong(9) '");
	TwAddVarRO(bar, "on", TW_TYPE_BOOL8, &oShader, " label='Oren Nayar(0) '");

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

	//transfer
	if (glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS){
		m_body->SetAngularMomentum(glm::vec3(0.9f,0,0));
	}
	if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
		m_body->SetAngularMomentum(glm::vec3(0,0.9f,0));
	}
	if (glfwGetKey(window, GLFW_KEY_T ) == GLFW_PRESS){
		m_body->SetAngularMomentum(glm::vec3(0,0,0.9f));
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