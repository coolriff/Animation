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

	
	isTexture = true;

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

	m_bodyMesh[0]->LoadMesh("../Models/teapot2.obj");
	m_bodyMesh[0]->setTexture("../Models/brick1.jpg",shaderDefault->GetProgramID());
	m_body[0]->SetPosition(glm::vec3(0,0,0));

	vLightDirGLM = glm::vec3(0,0,-1);
	ambientColorGLM = glm::vec3(1,1,1);
	specularColorGLM =glm::vec3(1,1,1);
	diffuseColorGLM = glm::vec3(1,1,1);
	ambientIntensityGLM =  0.2f;
	specularIntensityGLM =  0.6f;
	diffuseIntensityGLM =  0.8f;
	specularShininessGLM =  64.0f;
	
	do{
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		if (stopTime)
		{
			delta = 0;
		}

		preDraw();

		m_body[0]->Update(delta);

		keyControl();

		glUseProgram(shaderDefault->GetProgramID());

		shaderDefault->findAllShaderID();
		shaderDefault->SetAll(vLightDirGLM,ambientColorGLM,specularColorGLM,diffuseColorGLM,ambientIntensityGLM,specularIntensityGLM,diffuseIntensityGLM,specularShininessGLM);

		m_physicsLabCamera->computeMatricesFromInputs(window);
		modelLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "model");
		viewLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "view");
		projLoc = glGetUniformLocation(shaderDefault->GetProgramID(), "projection");
		m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

		update(m_body[0]->GetTransformationMatrix(), shaderDefault->GetProgramID());
		m_bodyMesh[0]->Render();

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
	createShaders(shaderDefault, "../Shader/defaultTexture.vs", "../Shader/defaultTexture.ps");
}

void RenderingLab1n2::createShaders(Shader *shader, std::string v, std::string p)
{
	std::string v1,f1;
	shader->readShaderFile(v,v1);
	shader->readShaderFile(p,f1);
	GLuint vertexShaderID = shader->makeShader(v1.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = shader->makeShader(f1.c_str(), GL_FRAGMENT_SHADER);
	shader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",shader->GetProgramID());
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

	TwAddVarRW(bar, "diffuseDirection", TW_TYPE_DIR3F, &vLightDirGLM, " label='vLightDir '");
	TwAddVarRW(bar, "lightPosition", TW_TYPE_COLOR3F, &ambientColorGLM, " label='ambientColor '");
	TwAddVarRW(bar, "lightIntensity",  TW_TYPE_COLOR3F, &specularColorGLM, " label='specularColor '");
	TwAddVarRW(bar, "lightIntensdsdsity",  TW_TYPE_COLOR3F, &diffuseColorGLM, " label='diffuseColor '");

	TwAddVarRW(bar, "materialAmbient", TW_TYPE_FLOAT, &ambientIntensityGLM, "step = 0.1" " label='ambientIntensity '");
	TwAddVarRW(bar, "materialDiffuse", TW_TYPE_FLOAT, &specularIntensityGLM, "step = 0.1" " label='specularIntensity '");
	TwAddVarRW(bar, "materiaSpecular", TW_TYPE_FLOAT, &diffuseIntensityGLM, "step = 0.1" " label='diffuseIntensity '");
	TwAddVarRW(bar, "roughness", TW_TYPE_FLOAT, &specularShininessGLM, "step = 0.1" " label='specularShininess '");
}

void RenderingLab1n2::update(glm::mat4 ModelMatrix, GLuint shaderProgramID)
{
	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
}

void RenderingLab1n2::keyControl()
{
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


	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET ) == GLFW_PRESS){
		m_bodyMesh[0]->isTextured = true;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET ) == GLFW_PRESS){
		m_bodyMesh[0]->isTextured = false;
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