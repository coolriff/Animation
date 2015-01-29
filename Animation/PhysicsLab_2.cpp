#include "PhysicsLab_2.h"

// #define WINDOW_WIDTH 1440
// #define WINDOW_HIGH 1080

#define WINDOW_WIDTH 1200
#define WINDOW_HIGH 900
#define MAX 36

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
	too_shader = new Shader();
	b_shader = new Shader();
	cylinderShader = new Shader();
	m_objectBuffer = new ObjectBuffer();
	m_objectBuffer2 = new ObjectBuffer();
	m_physicsLabCamera = new PhysicsLabCamera();
	cube = new Cube(glm::vec3(0,0,0), glm::quat());
	centre_of_mess = glm::vec3(0,0,0);
	stopTime = false;
	useForce = false;
	applyForcePoint = glm::vec3(0,0,0);
	applyForceF = glm::vec3(0,0,0); 
	tooShader = false;
	stdShader = true;
	bShader = false;
	MMShader = false;
	shaderType = STANDARD;
	createMesh = new CreateMesh();
	createMesh2 = new CreateMesh();
	cylinder = new Cylinder(2, 0.5, 0.5, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0),16);
	cylinderPos = glm::vec3(-4,0,0);
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

	directionalLightDirection = glm::vec3(0, 0, -1);

	

	createMesh->createCubeMesh();
	m_objectBuffer->GenerateVBO(createMesh->vertices,createMesh->colors,createMesh->normals);
	m_objectBuffer->LinkBufferToShaderWithNormal(too_shader->GetProgramID());
	m_objectBuffer->LinkBufferToShaderWithNormal(m_shader->GetProgramID());
	m_objectBuffer->LinkBufferToShaderWithNormal(b_shader->GetProgramID());
	m_objectBuffer->LinkBufferToShaderWithNormal(cylinderShader->GetProgramID());

	//cylinder->generateObjectBuffer(cylinderShader->GetProgramID());

	printf("Total Points for box object %d", cube->m_points.size());

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


		if (glfwGetKey(window, GLFW_KEY_8 ) == GLFW_PRESS){
			shaderType = STANDARD;
			stdShader = true;
			tooShader = false;
			bShader = false;
			MMShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_9 ) == GLFW_PRESS){
			shaderType = CARTOON;
			stdShader = false;
			tooShader = true;
			bShader = false;
			MMShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_0 ) == GLFW_PRESS){
			shaderType = WHATEVER;
			stdShader = false;
			tooShader = false;
			bShader = true;
			MMShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_7 ) == GLFW_PRESS){
			shaderType = NUMBER4;
			stdShader = false;
			tooShader = false;
			bShader = false;
			MMShader = true;
		}


		switch (shaderType)
		{
		case PhysicsLab_2::STANDARD:
			glUseProgram(m_shader->GetProgramID());
			break;
		case PhysicsLab_2::CARTOON:
			glUseProgram(too_shader->GetProgramID());
			break;
		case PhysicsLab_2::WHATEVER:
			glUseProgram(b_shader->GetProgramID());
			break;
		case PhysicsLab_2::NUMBER4:
			glUseProgram(cylinderShader->GetProgramID());
			break;
		}

		if (shaderType == STANDARD)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
			update(cube->GetTransformationMatrix(),m_shader->GetProgramID());
			
		}

		if (shaderType == CARTOON)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(too_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(too_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(too_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
			update(cube->GetTransformationMatrix(),too_shader->GetProgramID());
			too_shader->SetDirectionalLight(directionalLightDirection);
		}

		if (shaderType == WHATEVER)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(b_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(b_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(b_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
			update(cube->GetTransformationMatrix(),b_shader->GetProgramID());
			b_shader->SetDirectionalLight(directionalLightDirection);
		}

		if (shaderType == NUMBER4)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(cylinderShader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(cylinderShader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(cylinderShader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
			update(cube->GetTransformationMatrix(),cylinderShader->GetProgramID());
			cylinderShader->SetDirectionalLight(directionalLightDirection);
		}


		keyControl();
		cube->Update(delta);
		//cylinder->update(glm::rotate(glm::translate(glm::mat4(1),glm::vec3(4,0,0)),1.0f,glm::vec3(1,0,0)), cylinderShader->GetProgramID());
		//cylinder->update(glm::mat4(1), cylinderShader->GetProgramID());

		updateVertices();
		centreOfMess();

		if (useForce)
		{
			cube->ApplyForce(cube->GetPosition(),glm::vec3(0,-9.81f,0));
		}

		//draw cube
		glBindVertexArray(m_objectBuffer->vao);
		glDrawArrays(GL_TRIANGLES, 0, createMesh->vertices.size());
		glBindVertexArray(0);

		//cylinder->draw();



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
	//http://paulbourke.net/geometry/polygonmesh/

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
	for(int i = 0; i < createMesh->vertices.size(); i++)
	{
		glm::vec3 t = createMesh->vertices.at(i);
		bp[i] = t * cube->GetOrientation() + cube->GetPosition();
	}

	std::vector<glm::vec3> box_vertices;

	for (int i=0; i<36; i++)
	{
		box_vertices.push_back(glm::vec3(bp[i].x, bp[i].y, bp[i].z));
	}

	cube->SetPoints(box_vertices);

	for (int i=0; i<cube->m_points.size(); i++)
	{
		boxPos[i] = cube->m_points.at(i);
	}
}

void PhysicsLab_2::rotateBody(float x, float y, float z)
{
 	glm::quat q(glm::vec3(x, y, z));
 	cube->SetOrientation(cube->GetOrientation() * q);
}

void PhysicsLab_2::translateBody(float x, float y, float z)
{
	cube->SetLinearMomentum(12.0f * glm::vec3(x,y,z) * cube->GetMass());
}


void PhysicsLab_2::initShaders()
{
	//cube
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("default.vs",vertexShaderSourceCode);
	m_shader->readShaderFile("default.ps",fragmentShaderSourceCode);
	GLuint vertexShaderID = m_shader->makeShader(vertexShaderSourceCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = m_shader->makeShader(fragmentShaderSourceCode.c_str(), GL_FRAGMENT_SHADER);
	m_shader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",m_shader->GetProgramID());

	std::string vertexShaderSourceCode_2,fragmentShaderSourceCode_2;
	too_shader->readShaderFile("too.vs",vertexShaderSourceCode_2);
	too_shader->readShaderFile("too.ps",fragmentShaderSourceCode_2);
	GLuint vertexShaderID_2 = too_shader->makeShader(vertexShaderSourceCode_2.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_2 = too_shader->makeShader(fragmentShaderSourceCode_2.c_str(), GL_FRAGMENT_SHADER);
	too_shader->makeShaderProgram(vertexShaderID_2,fragmentShaderID_2);
	printf("vertexShaderID is %d\n",vertexShaderID_2);
	printf("fragmentShaderID is %d\n",fragmentShaderID_2);
	printf("shaderProgramID is %d\n",too_shader->GetProgramID());

	std::string vertexShaderSourceCode_3,fragmentShaderSourceCode_3;
	b_shader->readShaderFile("BlinnPhong.vs",vertexShaderSourceCode_3);
	b_shader->readShaderFile("BlinnPhong.ps",fragmentShaderSourceCode_3);
	GLuint vertexShaderID_3 = b_shader->makeShader(vertexShaderSourceCode_3.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_3 = b_shader->makeShader(fragmentShaderSourceCode_3.c_str(), GL_FRAGMENT_SHADER);
	b_shader->makeShaderProgram(vertexShaderID_3,fragmentShaderID_3);
	printf("vertexShaderID is %d\n",vertexShaderID_3);
	printf("fragmentShaderID is %d\n",fragmentShaderID_3);
	printf("shaderProgramID is %d\n",b_shader->GetProgramID());

	std::string vertexShaderSourceCodeCy,fragmentShaderSourceCodeCy;
	cylinderShader->readShaderFile("Gooch.vs",vertexShaderSourceCodeCy);
	cylinderShader->readShaderFile("Gooch.ps",fragmentShaderSourceCodeCy);
	GLuint vertexShaderIDCy = cylinderShader->makeShader(vertexShaderSourceCodeCy.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderIDCy = cylinderShader->makeShader(fragmentShaderSourceCodeCy.c_str(), GL_FRAGMENT_SHADER);
	cylinderShader->makeShaderProgram(vertexShaderIDCy,fragmentShaderIDCy);
	printf("vertexShaderID is %d\n",vertexShaderIDCy);
	printf("fragmentShaderID is %d\n",fragmentShaderIDCy);
	printf("shaderProgramID is %d\n",cylinderShader->GetProgramID());
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

	TwAddVarRO(bar, "mm", TW_TYPE_BOOL8, &MMShader, " label='MM(7) '");
	TwAddVarRO(bar, "std", TW_TYPE_BOOL8, &stdShader, " label='Default(8) '");
	TwAddVarRO(bar, "Too", TW_TYPE_BOOL8, &tooShader, " label='Cartoon(9) '");
	TwAddVarRO(bar, "bs", TW_TYPE_BOOL8, &bShader, " label='BlinnPhong(0) '");

	TwAddVarRW(bar, "l1", TW_TYPE_DIR3F, &directionalLightDirection, " label='Dir: '");

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
	TwAddVarRW(bar, "force", TW_TYPE_DIR3F, &applyForcePoint, " label='Force Pos: '");
	TwAddVarRW(bar, "forceD", TW_TYPE_DIR3F, &applyForceF, " label='Force Dir: '");
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
		cube->SetAngularMomentum(glm::vec3(1,0,0));
	}
	if (glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS){
		cube->SetAngularMomentum(glm::vec3(0,1,0));
	}
	if (glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS){
		cube->SetAngularMomentum(glm::vec3(0,0,1));
	}
	if (glfwGetKey(window, GLFW_KEY_4 ) == GLFW_PRESS){
		cube->SetAngularMomentum(glm::vec3(0,0,0));
	}

	
	if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
		cube->SetAngularMomentum(applyForcePoint);
	}

	if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS){
		cube->ApplyForce(applyForcePoint,applyForceF);
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

