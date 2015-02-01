#include "PhysicsLab_2.h"

// #define WINDOW_WIDTH 1440
// #define WINDOW_HIGH 1080



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
	m_objectBuffer = new ObjectBuffer();
	m_objectBuffer2 = new ObjectBuffer();
	m_physicsLabCamera = new PhysicsLabCamera();
	stopTime = false;
	useForce = false;
	applyForcePoint = glm::vec3(0,0,0);
	applyForceF = glm::vec3(0,0,0); 
	tooShader = false;
	stdShader = true;
	bShader = false;
	MMShader = false;
	shaderType = STANDARD;

	for (int i=0; i<MAXOBJECT; i++)
	{
		cubes[i] = new Cube();
		cubesMesh[i] = new CreateMesh();
		cubesBuffer[i] = new ObjectBuffer();
		boundingSpheres[i] = new CreateMesh();
		boundingSphereBuffers[i] = new ObjectBuffer();
		AABBcubes[i] = new Cube();
		AABBMeshs[i] = new CreateMesh();
		AABBBuffers[i] = new ObjectBuffer();
	}
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

	for (int i=0; i<MAXOBJECT; i++)
	{
		float f1 = -7.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(7.0f-(-7.0f))));
		float f2 = -7.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(7.0f-(-7.0f))));
		float f3 = -7.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(7.0f-(-7.0f))));

		cubes[i]->m_position = glm::vec3(f1,f2,f3);
		cubes[i]->m_orientation = glm::quat();
	}

	for (int i=0; i<MAXOBJECT; i++)
	{
		cubesMesh[i]->createCubeMesh();
		cubesBuffer[i]->GenerateVBO(cubesMesh[i]->vertices, cubesMesh[i]->colors, cubesMesh[i]->normals);
		cubesBuffer[i]->LinkBufferToShaderWithNormal(m_shader->GetProgramID());
		cubesBuffer[i]->LinkBufferToShaderWithNormal(b_shader->GetProgramID());
		cubesBuffer[i]->LinkBufferToShaderWithNormal(too_shader->GetProgramID());
	}

	//AABB
	for (int i=0; i<MAXOBJECT; i++)
	{
		AABBMeshs[i]->createCubeMesh();
		AABBBuffers[i]->GenerateVBO(AABBMeshs[i]->vertices, AABBMeshs[i]->newColorsAABB, AABBMeshs[i]->normals);
		AABBBuffers[i]->LinkBufferToShaderWithNormal(b_shader->GetProgramID());
	}



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
			//glUseProgram(cylinderShader->GetProgramID());
			break;
		}

		updateInBox();

		//20%
		if (shaderType == STANDARD)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			m_shader->SetDirectionalLight(directionalLightDirection);

			if (cubes[0]->distanceFromCentreMessToPoint)
			{
				for (int i=0; i<MAXOBJECT; i++)
				{
					boundingSphereBuffers[i]->ChangeColors(boundingSpheres[i]->colors);
				}
				distanceCheck();
			}

			for (int i=0; i<MAXOBJECT; i++)
			{
				update(cubes[i]->GetTransformationMatrix(),m_shader->GetProgramID());
				draw(cubesBuffer[i]->vao, cubesMesh[i]->vertices.size());

				update(cubes[i]->GetTransformationMatrix(),m_shader->GetProgramID());
				drawLine(boundingSphereBuffers[i]->vao, boundingSpheres[i]->vertices.size());
			}

		}

		if (shaderType == CARTOON)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(too_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(too_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(too_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			too_shader->SetDirectionalLight(directionalLightDirection);

			for (int i=0; i<MAXOBJECT; i++)
			{
				update(cubes[i]->GetTransformationMatrix(),m_shader->GetProgramID());
				draw(cubesBuffer[i]->vao, cubesMesh[i]->vertices.size());

				update(cubes[i]->GetTransformationMatrix(),m_shader->GetProgramID());
				drawLine(boundingSphereBuffers[i]->vao, boundingSpheres[i]->vertices.size());
			}
		}

		if (shaderType == WHATEVER)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(b_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(b_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(b_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			b_shader->SetDirectionalLight(directionalLightDirection);

			UpdatingAABBMaxMin();

			for (int i=0; i<MAXOBJECT; i++)
			{
				update(cubes[i]->GetTransformationMatrix(),b_shader->GetProgramID());
				draw(cubesBuffer[i]->vao, cubesMesh[i]->vertices.size());

				glm::mat4 tempPos = glm::mat4(1);
				tempPos[3][0] = cubes[i]->m_position.x;
				tempPos[3][1] = cubes[i]->m_position.y;
				tempPos[3][2] = cubes[i]->m_position.z;

				//float tempF = cubes[i]->distanceFromCentreMessToPoint * 2;

				glm::mat4 tempScale = glm::scale(glm::vec3(cubes[i]->maxAABBx  * 2,cubes[i]->maxAABBy * 2,cubes[i]->maxAABBz * 2));

				update((tempPos * glm::mat4(1) * tempScale),b_shader->GetProgramID());
				drawLine(AABBBuffers[i]->vao, AABBMeshs[i]->vertices.size());
			}
		}

		if (shaderType == NUMBER4)
		{
// 			m_physicsLabCamera->computeMatricesFromInputs(window);
// 			GLuint modelLoc = glGetUniformLocation(cylinderShader->GetProgramID(), "model");
// 			GLuint viewLoc = glGetUniformLocation(cylinderShader->GetProgramID(), "view");
// 			GLuint projLoc = glGetUniformLocation(cylinderShader->GetProgramID(), "projection");
// 			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
// 			update(cube->GetTransformationMatrix(),cylinderShader->GetProgramID());
// 			draw(m_objectBuffer->vao,createMesh->vertices.size());
// 			update(cube2->GetTransformationMatrix(),cylinderShader->GetProgramID());
// 			draw(m_objectBuffer2->vao,createMesh2->vertices.size());	
// 			cylinderShader->SetDirectionalLight(directionalLightDirection);
		}

		keyControl();

		for (int i=0; i<MAXOBJECT; i++)
		{
			cubes[i]->Update(delta);
			cubes[i]->updateCurrentVertices(MAX,cubesMesh[i]->vertices);
			cubes[i]->CalculateCentreOfMess(MAX);

			if (!cubes[i]->distanceFromCentreMessToPoint)
			{
				cubes[i]->CalculateDistanceFromCentreOfMessToPoint();
				boundingSpheres[i]->createBoundingSphereMesh(cubes[i]->distanceFromCentreMessToPoint, 10);
				boundingSphereBuffers[i]->GenerateVBO(boundingSpheres[i]->vertices,boundingSpheres[i]->colors,boundingSpheres[i]->normals);
				boundingSphereBuffers[i]->LinkBufferToShaderWithNormal(m_shader->GetProgramID());
				boundingSphereBuffers[i]->LinkBufferToShaderWithNormal(too_shader->GetProgramID());
				boundingSphereBuffers[i]->LinkBufferToShaderWithNormal(b_shader->GetProgramID());
			}
		}

		//check distance and change colors


		if (glfwGetKey(window, GLFW_KEY_5 ) == GLFW_PRESS){
			for (int i=0; i<MAXOBJECT; i++)
			{
				boundingSphereBuffers[i]->ChangeColors(boundingSpheres[i]->newColors);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_6 ) == GLFW_PRESS){
			for (int i=0; i<MAXOBJECT; i++)
			{
				boundingSphereBuffers[i]->ChangeColors(boundingSpheres[i]->colors);
			}
		}

		if (useForce)
		{
			for (int i=0; i<MAXOBJECT; i++)
			{
				cubes[i]->ApplyForce(cubes[i]->GetPosition(),glm::vec3(0,-9.81f,0));
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

void PhysicsLab_2::update8VerticesOnCube()
{
// 	for (int i=0; i<cube->m_points.size(); i++)
// 	{
// 		boxPos[i] = cube->m_points.at(i);
// 	}
}

void PhysicsLab_2::rotateBody(float x, float y, float z)
{
 	glm::quat q(glm::vec3(x, y, z));
 	//cube->SetOrientation(cube->GetOrientation() * q);
}

void PhysicsLab_2::translateBody(float x, float y, float z)
{
	cubes[0]->SetLinearMomentum(12.0f * glm::vec3(x,y,z) * cubes[0]->GetMass());
}


void PhysicsLab_2::initShaders()
{
	//cube
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("BlinnPhong.vs",vertexShaderSourceCode);
	m_shader->readShaderFile("BlinnPhong.ps",fragmentShaderSourceCode);
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

// 	std::string vertexShaderSourceCodeCy,fragmentShaderSourceCodeCy;
// 	cylinderShader->readShaderFile("Gooch.vs",vertexShaderSourceCodeCy);
// 	cylinderShader->readShaderFile("Gooch.ps",fragmentShaderSourceCodeCy);
// 	GLuint vertexShaderIDCy = cylinderShader->makeShader(vertexShaderSourceCodeCy.c_str(), GL_VERTEX_SHADER);
// 	GLuint fragmentShaderIDCy = cylinderShader->makeShader(fragmentShaderSourceCodeCy.c_str(), GL_FRAGMENT_SHADER);
// 	cylinderShader->makeShaderProgram(vertexShaderIDCy,fragmentShaderIDCy);
// 	printf("vertexShaderID is %d\n",vertexShaderIDCy);
// 	printf("fragmentShaderID is %d\n",fragmentShaderIDCy);
// 	printf("shaderProgramID is %d\n",cylinderShader->GetProgramID());
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

 	TwAddVarRW(bar, "Camera", TW_TYPE_DIR3F, &m_physicsLabCamera->position, " label='Camera Pos: '");

// 	TwAddVarRO(bar, "boxPoint0", TW_TYPE_DIR3F, &cubes[0]->m_points.at(0), " label='p0: '");
// 	TwAddVarRO(bar, "boxPoint1", TW_TYPE_DIR3F, &cubes[0]->m_points.at(1), " label='p1: '");
// 	TwAddVarRO(bar, "boxPoint2", TW_TYPE_DIR3F, &cubes[0]->m_points.at(2), " label='p2: '");
// 	TwAddVarRO(bar, "boxPoint3", TW_TYPE_DIR3F, &cubes[0]->m_points.at(3), " label='p3: '");
// 	TwAddVarRO(bar, "boxPoint4", TW_TYPE_DIR3F, &cubes[0]->m_points.at(4), " label='p4: '");
// 	TwAddVarRO(bar, "boxPoint5", TW_TYPE_DIR3F, &cubes[0]->m_points.at(5), " label='p5: '");
// 	TwAddVarRO(bar, "boxPoint6", TW_TYPE_DIR3F, &cubes[0]->m_points.at(6), " label='p6: '");
// 	TwAddVarRO(bar, "boxPoint7", TW_TYPE_DIR3F, &cubes[0]->m_points.at(7), " label='p7: '");
// 	TwAddVarRO(bar, "cmass", TW_TYPE_DIR3F, &cube->centre_of_mess, " label='centre of mess: '");
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
		rotateBody(-0.001f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_U ) == GLFW_PRESS){
		rotateBody(0.001f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_H ) == GLFW_PRESS){
		rotateBody(0.0f, -0.001f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_J ) == GLFW_PRESS){
		rotateBody(0.0f, 0.001f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_N ) == GLFW_PRESS){
		rotateBody(0.0f, 0.0f, -0.001f);
	}
	if (glfwGetKey(window, GLFW_KEY_M ) == GLFW_PRESS){
		rotateBody(0.0f, 0.0f, 0.001f);
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

// 	if (glfwGetKey(window, GLFW_KEY_1 ) == GLFW_PRESS){
// 		cube->SetAngularMomentum(glm::vec3(1,0,0));
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_2 ) == GLFW_PRESS){
// 		cube->SetAngularMomentum(glm::vec3(0,1,0));
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_3 ) == GLFW_PRESS){
// 		cube->SetAngularMomentum(glm::vec3(0,0,1));
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_4 ) == GLFW_PRESS){
// 		cube->SetAngularMomentum(glm::vec3(0,0,0));
// 	}
// 
// 	
// 	if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
// 		cube->SetAngularMomentum(applyForcePoint);
// 	}
// 
	if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS)
	{
		
		for (int i=0; i<MAXOBJECT; i++)
		{
			float p1 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f-(-1.5f))));
			float p2 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f-(-1.5f))));
			float p3 = -1.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.5f-(-1.5f))));

			float f1 = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50.0f-(-50.0f))));
			float f2 = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50.0f-(-50.0f))));
			float f3 = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50.0f-(-50.0f))));

			cubes[i]->ApplyForce(glm::vec3(p1,p2,p3),glm::vec3(f1,f2,f3));
			cubes[i]->SetOrientation(cubes[i]->GetOrientation() * glm::quat(glm::vec3(p1,p2,p3)));
			//cubes[i]->SetLinearMomentum(12.0f * glm::vec3(p1,p2,p3) * cubes[i]->GetMass());
		}
	}

	//dt
	if (glfwGetKey(window, GLFW_KEY_P ) == GLFW_PRESS){
		stopTime = true;
	}

	if (glfwGetKey(window, GLFW_KEY_O ) == GLFW_PRESS){
		stopTime = false;
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

void PhysicsLab_2::draw(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, size);
	glBindVertexArray(0);
}

void PhysicsLab_2::drawLine(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, size);
	glBindVertexArray(0);
}

void PhysicsLab_2::updateInBox()
{
	for (int i=0; i<MAXOBJECT; i++)
	{
		if (cubes[i]->m_position.x < -8)
		{
			cubes[i]->m_linearMomentum.x *= -1.0f;
		}
		if (cubes[i]->m_position.x > 8)
		{
			cubes[i]->m_linearMomentum.x *= -1.0f;
		}

		if (cubes[i]->m_position.y < -8)
		{
			cubes[i]->m_linearMomentum.y *= -1.0f;
		}
		if (cubes[i]->m_position.y > 8)
		{
			cubes[i]->m_linearMomentum.y *= -1.0f;
		}

		if (cubes[i]->m_position.z < -8)
		{
			cubes[i]->m_linearMomentum.z *= -1.0f;
		}
		if (cubes[i]->m_position.z > 8)
		{
			cubes[i]->m_linearMomentum.z *= -1.0f;
		}
	}
}

void PhysicsLab_2::distanceCheck()
{
	for (int i=0; i<MAXOBJECT; i++)
	{
		for (int j=0; j<MAXOBJECT; j++)
		{
			if (i != j)
			{
				float dis = glm::sqrt(
					((cubes[i]->centre_of_mess.x - cubes[j]->centre_of_mess.x) * (cubes[i]->centre_of_mess.x - cubes[j]->centre_of_mess.x)) + 
					((cubes[i]->centre_of_mess.y - cubes[j]->centre_of_mess.y) * (cubes[i]->centre_of_mess.y - cubes[j]->centre_of_mess.y)) + 
					((cubes[i]->centre_of_mess.z - cubes[j]->centre_of_mess.z) * (cubes[i]->centre_of_mess.z - cubes[j]->centre_of_mess.z))
					);

				dis = glm::abs(dis);

				if (dis < (cubes[i]->distanceFromCentreMessToPoint + cubes[j]->distanceFromCentreMessToPoint))
				{
					boundingSphereBuffers[i]->ChangeColors(boundingSpheres[i]->newColors);
					boundingSphereBuffers[j]->ChangeColors(boundingSpheres[j]->newColors);
					//stopTime = true;
				}
			}
		}
	}
}

void PhysicsLab_2::UpdatingAABBMaxMin()
{
	for (int i=0; i<MAXOBJECT; i++)
	{
		cubes[i]->maxAABBx = 0.0f;
		cubes[i]->maxAABBy = 0.0f;
		cubes[i]->maxAABBz = 0.0f;
		for (int j=0; j<cubes[i]->m_points.size(); j++)
		{
			glm::vec3 p = cubes[i]->m_points[j];
			if (glm::abs(p.x - cubes[i]->m_position.x) >= cubes[i]->maxAABBx)
			{
				cubes[i]->maxAABBx = glm::abs(p.x - cubes[i]->m_position.x);
				if (p.x > cubes[i]->m_position.x)
				{
					cubes[i]->maxX = p.x;
					cubes[i]->minX = p.x - (cubes[i]->maxAABBx * 2);
				}
				else
				{
					cubes[i]->minX = p.x;
					cubes[i]->maxX = p.x + (cubes[i]->maxAABBx * 2);
				}
			}
			if (glm::abs(p.y - cubes[i]->m_position.y) >= cubes[i]->maxAABBy)
			{
				cubes[i]->maxAABBy = glm::abs(p.y - cubes[i]->m_position.y);
				if (p.y > cubes[i]->m_position.y)
				{
					cubes[i]->maxY = p.y;
					cubes[i]->minY = p.y - (cubes[i]->maxAABBy * 2);
				}
				else
				{
					cubes[i]->minY = p.y;
					cubes[i]->maxY = p.y + (cubes[i]->maxAABBy * 2);
				}
			}
			if (glm::abs(p.z - cubes[i]->m_position.z) >= cubes[i]->maxAABBz)
			{
				cubes[i]->maxAABBz = glm::abs(p.z - cubes[i]->m_position.z);
				if (p.z > cubes[i]->m_position.z)
				{
					cubes[i]->maxZ = p.z;
					cubes[i]->minZ = p.z - (cubes[i]->maxAABBz * 2);
				}
				else
				{
					cubes[i]->minZ = p.z;
					cubes[i]->maxZ = p.z + (cubes[i]->maxAABBz * 2);
				}
			}
		}
	}
}

void PhysicsLab_2::computingAABBMaxMin()
{
	for (int i=0; i<MAXOBJECT; i++)
	{
		for (int j=0; j<cubes[i]->m_points.size(); j++)
		{
			glm::vec3 p = cubes[i]->m_points[j];
			if ((p.x > cubes[i]->m_position.x) && glm::abs(p.x - cubes[i]->m_position.x) >= cubes[i]->maxAABBx)
			{
				cubes[i]->maxAABBx = glm::abs(p.x - cubes[i]->m_position.x);
				cubes[i]->maxX = p.x;
			}
			if ((p.x < cubes[i]->m_position.x) && glm::abs(p.x - cubes[i]->m_position.x) <= cubes[i]->maxAABBx)
			{
				cubes[i]->maxAABBx = glm::abs(p.x - cubes[i]->m_position.x);
				cubes[i]->maxX = p.x;
			}
			if (glm::abs(p.y - cubes[i]->m_position.y) >= cubes[i]->maxAABBy)
			{
				cubes[i]->maxAABBy = glm::abs(p.y - cubes[i]->m_position.y);
			}
			if (glm::abs(p.z - cubes[i]->m_position.z) >= cubes[i]->maxAABBz)
			{
				cubes[i]->maxAABBz = glm::abs(p.z - cubes[i]->m_position.z);
			}
		}
	}
}



