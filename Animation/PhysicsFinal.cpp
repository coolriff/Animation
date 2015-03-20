#include "PhysicsFinal.h"


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

PhysicsFinal::PhysicsFinal(void)
{
	m_shader = new Shader();
	m_camera = new PhysicsLabCamera();
}


PhysicsFinal::~PhysicsFinal(void)
{
	cleanUp();
}


void PhysicsFinal::run(void)
{
	setupGlfwGlew();
	initShaders();
	initTweakBar();

	do{
		preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_camera->computeMatricesFromInputs(window);

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");

		m_camera->handleMVP(modelLoc, viewLoc, projLoc);

		TwDraw();
		// Swap buffers
		glfwSwapBuffers(getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(getWindow()) == 0 );
}


void PhysicsFinal::initShaders()
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

void PhysicsFinal::setupGlfwGlew()
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

void PhysicsFinal::cleanUp()
{
	glfwTerminate();
}

void PhysicsFinal::preDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* PhysicsFinal::getWindow()
{
	return window;
}

void PhysicsFinal::initTweakBar()
{
	TwBar * bar;
	bar = TwNewBar("Simulation");
	TwDefine(" Simulation size='300 400' ");
// 
// 	TwAddVarRO(bar, "mm", TW_TYPE_BOOL8, &MMShader, " label='MM(7) '");
// 	TwAddVarRO(bar, "std", TW_TYPE_BOOL8, &stdShader, " label='Default(8) '");
// 	TwAddVarRO(bar, "Too", TW_TYPE_BOOL8, &tooShader, " label='Cartoon(9) '");
// 	TwAddVarRO(bar, "bs", TW_TYPE_BOOL8, &bShader, " label='BlinnPhong(0) '");
// 
// 	TwAddVarRW(bar, "l1", TW_TYPE_DIR3F, &directionalLightDirection, " label='Dir: '");
// 
// 	TwAddVarRW(bar, "Camera", TW_TYPE_DIR3F, &m_physicsLabCamera->position, " label='Camera Pos: '");
// 
// 	TwAddVarRO(bar, "boxPoint0", TW_TYPE_DIR3F, &cubes[0]->m_points.at(0), " label='p0: '");
// 	TwAddVarRO(bar, "boxPoint1", TW_TYPE_DIR3F, &cubes[0]->m_points.at(1), " label='p1: '");
// 	TwAddVarRO(bar, "boxPoint2", TW_TYPE_DIR3F, &cubes[0]->m_points.at(2), " label='p2: '");
// 	TwAddVarRO(bar, "boxPoint3", TW_TYPE_DIR3F, &cubes[0]->m_points.at(3), " label='p3: '");
// 	TwAddVarRO(bar, "boxPoint4", TW_TYPE_DIR3F, &cubes[0]->m_points.at(4), " label='p4: '");
// 	TwAddVarRO(bar, "boxPoint5", TW_TYPE_DIR3F, &cubes[0]->m_points.at(5), " label='p5: '");
// 	TwAddVarRO(bar, "boxPoint6", TW_TYPE_DIR3F, &cubes[0]->m_points.at(6), " label='p6: '");
// 	TwAddVarRO(bar, "boxPoint7", TW_TYPE_DIR3F, &cubes[0]->m_points.at(7), " label='p7: '");
// 	TwAddVarRO(bar, "cmass", TW_TYPE_DIR3F, &cubes[0]->centre_of_mass, " label='centre of mass: '");
// 	TwAddVarRW(bar, "cpos", TW_TYPE_DIR3F, &cubes[0]->m_position, " label='Pos: '");
// 	TwAddVarRO(bar, "AABBmAX", TW_TYPE_DIR3F, &cubes[0]->AABBmax, " label='AABBmax: '");
// 	TwAddVarRO(bar, "AABBmin", TW_TYPE_DIR3F, &cubes[0]->AABBmin, " label='AABBmin: '");
// 	TwAddVarRO(bar, "maxAABBx", TW_TYPE_FLOAT, &cubes[0]->maxAABBx, " label='maxAABBx: '");
// 	TwAddVarRO(bar, "maxAABBy", TW_TYPE_FLOAT, &cubes[0]->maxAABBy, " label='maxAABBy: '");
// 	TwAddVarRO(bar, "maxAABBz", TW_TYPE_FLOAT, &cubes[0]->maxAABBz, " label='maxAABBz: '");
// 
// 	TwAddVarRW(bar, "force", TW_TYPE_DIR3F, &applyForcePoint, " label='Force Pos: '");
// 	TwAddVarRW(bar, "forceD", TW_TYPE_DIR3F, &applyForceF, " label='Force Dir: '");
}
