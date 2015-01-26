#include "PhysicsLab_2.h"

#define WINDOW_WIDTH 1440
#define WINDOW_HIGH 1080

// #define WINDOW_WIDTH 800
// #define WINDOW_HIGH 600
#define MAX 36

glm::vec3 vertices[] = {
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f, 1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f, 1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f,-1.0f, 1.0f)
};

// Create a color array that identifies the colors of each vertex (format R, G, B, A)
GLfloat colors[] = {
	0.583f,  0.771f,  0.014f,  1.0f,
	0.609f,  0.115f,  0.436f,  1.0f,
	0.327f,  0.483f,  0.844f,  1.0f,
	0.822f,  0.569f,  0.201f,  1.0f,
	0.435f,  0.602f,  0.223f,  1.0f,
	0.310f,  0.747f,  0.185f,  1.0f,
	0.597f,  0.770f,  0.761f,  1.0f,
	0.559f,  0.436f,  0.730f,  1.0f,
	0.359f,  0.583f,  0.152f,  1.0f,
	0.483f,  0.596f,  0.789f,  1.0f,
	0.559f,  0.861f,  0.639f,  1.0f,
	0.195f,  0.548f,  0.859f,  1.0f,
	0.014f,  0.184f,  0.576f,  1.0f,
	0.771f,  0.328f,  0.970f,  1.0f,
	0.406f,  0.615f,  0.116f,  1.0f,
	0.676f,  0.977f,  0.133f,  1.0f,
	0.971f,  0.572f,  0.833f,  1.0f,
	0.140f,  0.616f,  0.489f,  1.0f,
	0.997f,  0.513f,  0.064f,  1.0f,
	0.945f,  0.719f,  0.592f,  1.0f,
	0.543f,  0.021f,  0.978f,  1.0f,
	0.279f,  0.317f,  0.505f,  1.0f,
	0.167f,  0.620f,  0.077f,  1.0f,
	0.347f,  0.857f,  0.137f,  1.0f,
	0.055f,  0.953f,  0.042f,  1.0f,
	0.714f,  0.505f,  0.345f,  1.0f,
	0.783f,  0.290f,  0.734f,  1.0f,
	0.722f,  0.645f,  0.174f,  1.0f,
	0.302f,  0.455f,  0.848f,  1.0f,
	0.225f,  0.587f,  0.040f,  1.0f,
	0.517f,  0.713f,  0.338f,  1.0f,
	0.053f,  0.959f,  0.120f,  1.0f,
	0.393f,  0.621f,  0.362f,  1.0f,
	0.673f,  0.211f,  0.457f,  1.0f,
	0.820f,  0.883f,  0.371f,  1.0f,
	0.982f,  0.099f,  0.879f,  1.0f
};

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


PhysicsLab_2::PhysicsLab_2(void)
{
	m_shader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_physicsLabCamera = new PhysicsLabCamera();
	box = new Box(glm::vec3(0,0,0), glm::quat());
	centre_of_mess = glm::vec3(0,0,0);
	stopTime = false;
	useForce = false;
}


PhysicsLab_2::~PhysicsLab_2(void)
{
	cleanUp();
	TwTerminate();
}


void PhysicsLab_2::run(void)
{
	setupGlfwGlew();

	initShaders();
	initTweakBar();

	// Create 3 vertices that make up a triangle that fits on the viewport 




	printf("Total Points for box object %d",box->points.size());

	std::vector<glm::vec3> box_InitVertices;

	for (int i=0; i<36; i++)
	{
		box_InitVertices.push_back(glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z));
	}

	m_objectBuffer->GenerateVBO(box_InitVertices,colors);
	m_objectBuffer->LinkBufferToShader(m_shader->GetProgramID());

	double lastTime = glfwGetTime();



	do{

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		if (stopTime)
		{
			delta = 0;
		}

		preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_physicsLabCamera->computeMatricesFromInputs(window);

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");

		m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

		keyControl();
		box->Update(delta);
		update(box->GetTransformationMatrix(),m_shader->GetProgramID());

		updateVertices();
		centreOfMess();

		if (useForce)
		{
			box->ApplyForce(box->GetPosition(),glm::vec3(0,-9.81f,0));
		}


		//draw
		glBindVertexArray(m_objectBuffer->vao);
		glDrawArrays(GL_TRIANGLES, 0, 12*3);
		glBindVertexArray(0);

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



void PhysicsLab_2::inertialTensor()
{

}


void PhysicsLab_2::centreOfMess()
{
	//Centroid of a 3D shell described by 3 vertex facets
	float area[MAX];

	glm::vec3 r[MAX];
	glm::vec3 up;
	glm::vec3 down;

	for (int i=0; i<MAX; i+=3)
	{
		glm::vec3 v1 = bp[i];
		glm::vec3 v2 = bp[i+1];
		glm::vec3 v3 = bp[i+2];

		r[i] = (v1 + v2 + v3) / 3.0f;
		area[i] = glm::length(glm::cross(v2-v1,v3-v1));
		up += r[i] * area[i];
		down += area[i];
	}
	centre_of_mess = up/down;
}



void PhysicsLab_2::updateVertices()
{
	for(int i = 0; i < MAX; i++)
	{
		bp[i] = glm::vec3(vertices[i]) * box->GetOrientation() + box->GetPosition();
	}

	std::vector<glm::vec3> box_vertices;

	for (int i=0; i<36; i++)
	{
		box_vertices.push_back(glm::vec3(bp[i].x, bp[i].y, bp[i].z));
	}

	box->SetPoints(box_vertices);

	for (int i=0; i<box->points.size(); i++)
	{
		boxPos[i] = box->points.at(i);
	}
}

void PhysicsLab_2::rotateBody(float x, float y, float z)
{
 	glm::quat q(glm::vec3(x, y, z));
 	box->SetOrientation(box->GetOrientation() * q);
}

void PhysicsLab_2::translateBody(float x, float y, float z)
{
	box->SetLinearMomentum(12.0f * glm::vec3(x,y,z) * box->GetMass());
}


void PhysicsLab_2::initShaders()
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

void PhysicsLab_2::setupGlfwGlew()
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

void PhysicsLab_2::cleanUp()
{
	glfwTerminate();
}

void PhysicsLab_2::preDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* PhysicsLab_2::getWindow()
{
	return window;
}

void PhysicsLab_2::initTweakBar()
{
	TwBar * bar;
	bar = TwNewBar("Simulation");
	TwDefine(" Simulation size='300 400' ");
	//TwDefine(" Simulation resizable=false ");
	//TwDefine(" Simulation position='0 0' ");

	//TwSetParam(bar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");

 	TwAddVarRO(bar, "Camera", TW_TYPE_DIR3F, &m_physicsLabCamera->position, " label='Camera Pos: '");

	TwAddVarRO(bar, "boxPoint0", TW_TYPE_DIR3F, &boxPos[0], " label='p0: '");
	TwAddVarRO(bar, "boxPoint1", TW_TYPE_DIR3F, &boxPos[1], " label='p1: '");
	TwAddVarRO(bar, "boxPoint2", TW_TYPE_DIR3F, &boxPos[2], " label='p2: '");
	TwAddVarRO(bar, "boxPoint3", TW_TYPE_DIR3F, &boxPos[3], " label='p3: '");
	TwAddVarRO(bar, "boxPoint4", TW_TYPE_DIR3F, &boxPos[4], " label='p4: '");
	TwAddVarRO(bar, "boxPoint5", TW_TYPE_DIR3F, &boxPos[5], " label='p5: '");
	TwAddVarRO(bar, "boxPoint6", TW_TYPE_DIR3F, &boxPos[6], " label='p6: '");
	TwAddVarRO(bar, "boxPoint7", TW_TYPE_DIR3F, &boxPos[7], " label='p7: '");
	TwAddVarRO(bar, "cmass", TW_TYPE_DIR3F, &centre_of_mess, " label='centre of mess: '");
 
// 	TwAddVarRO(bar, "Spread", TW_TYPE_FLOAT, &spread, " label='Spread(O,P): '");
// 
// 	TwAddVarRO(bar, "centripetal", TW_TYPE_BOOL8, &centripetal, " label='Centripetal(Z): '");
// 	TwAddVarRO(bar, "waterfall", TW_TYPE_BOOL8, &waterfall, " label='Waterfall(X): '");
// 	TwAddVarRO(bar, "waveFountain", TW_TYPE_BOOL8, &waveFountain, " label='WaveFountain(C): '");
// 	TwAddVarRO(bar, "gravityOn", TW_TYPE_BOOL8, &gravityOn, " label='GravityOn(Q): '");
// 	TwAddVarRO(bar, "stopButton", TW_TYPE_BOOL8, &stopButton, " label='StopButton(Space): '");
// 
// 	TwAddVarRO(bar, "Point1 X", TW_TYPE_FLOAT, &blackhole.x, "group ='Blackhole' label='X(R,T):'");
// 	TwAddVarRO(bar, "Point1 Y", TW_TYPE_FLOAT, &blackhole.y, "group ='Blackhole' label='Y(F,G):'");
// 	TwAddVarRO(bar, "Point1 Z", TW_TYPE_FLOAT, &blackhole.z, "group ='Blackhole' label='Z(V,B):'");
// 
// 	TwAddVarRO(bar, "waveFountainAngle X", TW_TYPE_FLOAT, &waveFountainAngle.x, "group ='WaveFountainAngle' label='X(7,8): '");
// 	TwAddVarRO(bar, "waveFountainAngle Y", TW_TYPE_FLOAT, &waveFountainAngle.y, "group ='WaveFountainAngle' label='Y(9,0): '");
// 	TwAddVarRO(bar, "waveFountainAngle Z", TW_TYPE_FLOAT, &waveFountainAngle.z, "group ='WaveFountainAngle' label='Z(): '");
// 
// 	TwAddVarRO(bar, "centripetalForce X", TW_TYPE_FLOAT, &centripetalForce.x, "group ='CentripetalForce' label='X(1,2): '");
// 	TwAddVarRO(bar, "centripetalForce Y", TW_TYPE_FLOAT, &centripetalForce.y, "group ='CentripetalForce' label='Y(3,4): '");
// 	TwAddVarRO(bar, "centripetalForce Z", TW_TYPE_FLOAT, &centripetalForce.z, "group ='CentripetalForce' label='Z(5,6): '");
// 
// 	TwAddVarRO(bar, "Gravity X", TW_TYPE_FLOAT, &gravity.x, "group ='Gravity' label='X(Y,U): '");
// 	TwAddVarRO(bar, "Gravity Y", TW_TYPE_FLOAT, &gravity.y, "group ='Gravity' label='Y(H,J): '");
// 	TwAddVarRO(bar, "Gravity Z", TW_TYPE_FLOAT, &gravity.z, "group ='Gravity' label='Z(N,M): '");

}

void PhysicsLab_2::update(glm::mat4 ModelMatrix, GLuint shaderProgramID)
{
	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
}

void PhysicsLab_2::keyControl()
{
	//rot
	if (glfwGetKey(window, GLFW_KEY_Y ) == GLFW_PRESS){
		rotateBody(-0.1f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_U ) == GLFW_PRESS){
		rotateBody(0.1f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_H ) == GLFW_PRESS){
		rotateBody(0.0f, -0.1f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_J ) == GLFW_PRESS){
		rotateBody(0.0f, 0.1f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_N ) == GLFW_PRESS){
		rotateBody(0.0f, 0.0f, -0.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_M ) == GLFW_PRESS){
		rotateBody(0.0f, 0.0f, 0.1f);
	}

	//transfer
	if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
		translateBody(-0.1f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_T ) == GLFW_PRESS){
		translateBody(0.1f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_F ) == GLFW_PRESS){
		translateBody(0.0f, -0.1f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_G ) == GLFW_PRESS){
		translateBody(0.0f, 0.1f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_V ) == GLFW_PRESS){
		translateBody(0.0f, 0.0f, -0.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_B ) == GLFW_PRESS){
		translateBody(0.0f, 0.0f, 0.1f);
	}

	if (glfwGetKey(window, GLFW_KEY_1 ) == GLFW_PRESS){
		box->SetAngularMomentum(glm::vec3(1,0,0));
	}
	if (glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS){
		box->SetAngularMomentum(glm::vec3(0,1,0));
	}
	if (glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS){
		box->SetAngularMomentum(glm::vec3(0,0,1));
	}
	if (glfwGetKey(window, GLFW_KEY_4 ) == GLFW_PRESS){
		box->SetAngularMomentum(glm::vec3(0,0,0));
	}

	//dt
	if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS){
		if (stopTime == false)
		{
			stopTime = true;
		}
		else
		{
			stopTime = false;
		}
	}

	// use force
	if (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS){
		if (useForce == false)
		{
			useForce = true;
		}
		else
		{
			useForce = false;
		}
	}

}

