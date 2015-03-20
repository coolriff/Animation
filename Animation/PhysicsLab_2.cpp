#include "PhysicsLab_2.h"

// typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
// void MouseButtonCB( GLFWwindow*,int button ,int action ,int mods)
// {
// 	TwEventMouseButtonGLFW( button , action );
// }
// 
// typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
// void MousePosCB(GLFWwindow*,double x ,double y)
// {
// 	TwEventMousePosGLFW( (int)x, (int)y );
// }
// 
// typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
// void KeyFunCB( GLFWwindow* window,int key,int scancode,int action,int mods)
// {
// 	TwEventKeyGLFW( key , action );
// 	TwEventCharGLFW( key  , action );
// }
// 
// typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
// void MouseScrollCB(  GLFWwindow* window, double x , double y )
// {
// 	TwEventMouseWheelGLFW( (int)y );
// }

std::vector<DemoResult> demoResult;

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
	gSortAxis = 0;

	for (int i=0; i<MAXOBJECT; i++)
	{
		cubes[i] = new Cube();
		cubesMesh[i] = new CreateMesh();
		cubesBuffer[i] = new ObjectBuffer();
		boundingSpheresCube[i] = new CreateMesh();
		boundingSphereCubeBuffers[i] = new ObjectBuffer();
		AABBcubes[i] = new Cube();
		AABBMeshs[i] = new CreateMesh();
		AABBBuffers[i] = new ObjectBuffer();
	}

	for (int i=0; i<MAXOBJECT * 8; i++)
	{
		boundingSpheres[i] = new CreateMesh();
		boundingSphereBuffers[i] = new ObjectBuffer();
		collidingPointMesh[i] = new CreateMesh();
		collidingPointBuffers[i] = new ObjectBuffer();
	}

	drawPoints = false;
	line = new Line();
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

	directionalLightDirection = glm::vec3(0, 0, -1);

	for (int i=0; i<MAXOBJECT; i++)
	{
		float f1 = -SPACE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(SPACE-(-SPACE))));
		float f2 = -SPACE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(SPACE-(-SPACE))));
		float f3 = -SPACE + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(SPACE-(-SPACE))));

		cubes[i]->m_position = glm::vec3(f1,f2,f3);
		cubes[i]->m_orientation = glm::quat();
		cubes[i]->ID = i;
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
		AABBBuffers[i]->GenerateVBO(AABBMeshs[i]->vertices, AABBMeshs[i]->greenColors, AABBMeshs[i]->normals);
		AABBBuffers[i]->LinkBufferToShaderWithNormal(b_shader->GetProgramID());
	}

	double lastTime = glfwGetTime();

	for (int i=0; i<MAXOBJECT; i++)
	{
		cubes[i]->Update(0.001f);
		cubes[i]->updateCurrentVertices(MAX,cubesMesh[i]->vertices);
		cubes[i]->CalculateCentreOfMess(MAX);

		if (!cubes[i]->distanceFromCentreMessToPoint)
		{
			cubes[i]->CalculateDistanceFromCentreOfMessToPoint();
			boundingSpheresCube[i]->createBoundingSphereMesh(cubes[i]->distanceFromCentreMessToPoint, 10);
			boundingSphereCubeBuffers[i]->GenerateVBO(boundingSpheresCube[i]->vertices,boundingSpheresCube[i]->colors,boundingSpheresCube[i]->normals);
			boundingSphereCubeBuffers[i]->LinkBufferToShaderWithNormal(m_shader->GetProgramID());

		}
	}

	for (int j=0; j<MAXOBJECT * 8; j++)
	{
		boundingSpheres[j]->createBoundingSphereMesh(0.05, 5);
		boundingSphereBuffers[j]->GenerateVBO(boundingSpheres[j]->vertices,boundingSpheres[j]->redColors,boundingSpheres[j]->normals);
		boundingSphereBuffers[j]->LinkBufferToShaderWithNormal(m_shader->GetProgramID());
		boundingSphereBuffers[j]->LinkBufferToShaderWithNormal(too_shader->GetProgramID());
		boundingSphereBuffers[j]->LinkBufferToShaderWithNormal(b_shader->GetProgramID());

		collidingPointMesh[j]->createBoundingSphereMesh(0.05, 5);
		collidingPointBuffers[j]->GenerateVBO(boundingSpheres[j]->vertices,boundingSpheres[j]->colors,boundingSpheres[j]->normals);
		collidingPointBuffers[j]->LinkBufferToShaderWithNormal(m_shader->GetProgramID());
		collidingPointBuffers[j]->LinkBufferToShaderWithNormal(too_shader->GetProgramID());
		collidingPointBuffers[j]->LinkBufferToShaderWithNormal(b_shader->GetProgramID());
	}
	
	v.push_back(glm::vec3());
	v.push_back(glm::vec3());
	s.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	s.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	line->GenerateBuffer(v, s);
	line->linkShader(m_shader->GetProgramID());

	initTweakBar();

	do{
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		if (stopTime)
		{
			delta = 0;
		}

		preDraw();

		keyControl();

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

		for (int i=0; i<MAXOBJECT; i++)
		{
			cubes[i]->Update(delta);
			cubes[i]->updateCurrentVertices(MAX,cubesMesh[i]->vertices);
			cubes[i]->CalculateCentreOfMess(MAX);
		}

		updateInBox();

		//ball detecction
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
					boundingSphereCubeBuffers[i]->ChangeColors(boundingSpheresCube[i]->colors);
					cubesBuffer[i]->ChangeColors(cubesMesh[i]->colors);
				}
				distanceCheck();
			}

			for (int i=0; i<MAXOBJECT; i++)
			{
				update(cubes[i]->GetTransformationMatrix(),m_shader->GetProgramID());
				draw(cubesBuffer[i]->vao, cubesMesh[i]->vertices.size());

				update(cubes[i]->GetTransformationMatrix(),m_shader->GetProgramID());
				drawLine(boundingSphereCubeBuffers[i]->vao, boundingSpheresCube[i]->vertices.size());

				if (drawPoints)
				{
					for (int j=0; j<cubes[i]->m_points.size(); j++)
					{
						glm::mat4 tPos = glm::mat4(1);
						tPos[3][0] = cubes[i]->m_points[j].x;
						tPos[3][1] = cubes[i]->m_points[j].y;
						tPos[3][2] = cubes[i]->m_points[j].z;

						update(tPos,m_shader->GetProgramID());
						draw(boundingSphereBuffers[j]->vao, boundingSpheres[j]->vertices.size());
					}
				}
			}

		}

		//9
		//AABB with SWEEP AND PRUNE
		if (shaderType == CARTOON)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(too_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(too_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(too_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			too_shader->SetDirectionalLight(directionalLightDirection);

			//BroadPhase update start
			UpdatingAABBMaxMin();
			for (int i=0; i<MAXOBJECT; i++)
			{
				AABBBuffers[i]->ChangeColors(AABBMeshs[i]->greenColors);
				cubesBuffer[i]->ChangeColors(cubesMesh[i]->colors);
			}

			computAABBOverLapWithSweepAndPrune();

			for (int i=0; i<MAXOBJECT; i++)
			{
				update(cubes[i]->GetTransformationMatrix(),too_shader->GetProgramID());
				draw(cubesBuffer[i]->vao, cubesMesh[i]->vertices.size());

				glm::mat4 tempPos = glm::mat4(1);
				tempPos[3][0] = cubes[i]->centre_of_mass.x;
				tempPos[3][1] = cubes[i]->centre_of_mass.y;
				tempPos[3][2] = cubes[i]->centre_of_mass.z;

				tempPos = glm::scale(tempPos,glm::vec3(cubes[i]->maxAABBx  * 2,cubes[i]->maxAABBy * 2,cubes[i]->maxAABBz * 2));

				update(tempPos,too_shader->GetProgramID());
				drawLine(AABBBuffers[i]->vao, AABBMeshs[i]->vertices.size());

				if (drawPoints)
				{
					for (int j=0; j<cubes[i]->m_points.size(); j++)
					{
						glm::mat4 tPos = glm::mat4(1);
						tPos[3][0] = cubes[i]->m_points[j].x;
						tPos[3][1] = cubes[i]->m_points[j].y;
						tPos[3][2] = cubes[i]->m_points[j].z;

						update(tPos,m_shader->GetProgramID());
						draw(boundingSphereBuffers[j]->vao, boundingSpheres[j]->vertices.size());
					}
				}

			}
			//BroadPhase update end
		}

		//AABB without SWEEP AND PRUNE
		if (shaderType == WHATEVER)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(b_shader->GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(b_shader->GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(b_shader->GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			b_shader->SetDirectionalLight(directionalLightDirection);

			//BroadPhase update start
			UpdatingAABBMaxMin();
			for (int i=0; i<MAXOBJECT; i++)
			{
				AABBBuffers[i]->ChangeColors(AABBMeshs[i]->blueColors);
				cubesBuffer[i]->ChangeColors(cubesMesh[i]->colors);
			}
			computAABBOverLap();
			//BroadPhase update end

			for (int i=0; i<MAXOBJECT; i++)
			{
				update(cubes[i]->GetTransformationMatrix(),b_shader->GetProgramID());
				drawLine(cubesBuffer[i]->vao, cubesMesh[i]->vertices.size());

				glm::mat4 tempPos = glm::mat4(1);
				tempPos[3][0] = cubes[i]->centre_of_mass.x;
				tempPos[3][1] = cubes[i]->centre_of_mass.y;
				tempPos[3][2] = cubes[i]->centre_of_mass.z;

				tempPos = glm::scale(tempPos,glm::vec3(cubes[i]->maxAABBx  * 2,cubes[i]->maxAABBy * 2,cubes[i]->maxAABBz * 2));

				update(tempPos,b_shader->GetProgramID());
				drawLine(AABBBuffers[i]->vao, AABBMeshs[i]->vertices.size());

				if (drawPoints)
				{
					for (int j=0; j<cubes[i]->m_points.size(); j++)
					{
						glm::mat4 tPos = glm::mat4(1);
						tPos[3][0] = cubes[i]->m_points[j].x;
						tPos[3][1] = cubes[i]->m_points[j].y;
						tPos[3][2] = cubes[i]->m_points[j].z;

						update(tPos,m_shader->GetProgramID());
						draw(boundingSphereBuffers[j]->vao, boundingSpheres[j]->vertices.size());
					}
				}

			}
		}

		if (shaderType == NUMBER4)
		{
			//TODO
		}


		if (glfwGetKey(window, GLFW_KEY_5 ) == GLFW_PRESS){
			for (int i=0; i<MAXOBJECT; i++)
			{
				boundingSphereBuffers[i]->ChangeColors(boundingSpheres[i]->redColors);
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
				//cubes[i]->m_linearMomentum.y *= -0.75f;
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
	cubes[0]->SetOrientation(cubes[0]->GetOrientation() * q);
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
	too_shader->readShaderFile("BlinnPhong.vs",vertexShaderSourceCode_2);
	too_shader->readShaderFile("BlinnPhong.ps",fragmentShaderSourceCode_2);
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
// 	glfwSetMouseButtonCallback( window , MouseButtonCB );
// 	// - Directly redirect GLFW mouse position events to AntTweakBar
// 	glfwSetCursorPosCallback( window , MousePosCB);
// 	// - Directly redirect GLFW mouse wheel events to AntTweakBar
// 	glfwSetScrollCallback( window , MouseScrollCB );
// 	// - Directly redirect GLFW key events to AntTweakBar
// 	glfwSetKeyCallback(window , KeyFunCB);


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

	TwAddVarRO(bar, "boxPoint0", TW_TYPE_DIR3F, &cubes[0]->m_points.at(0), " label='p0: '");
	TwAddVarRO(bar, "boxPoint1", TW_TYPE_DIR3F, &cubes[0]->m_points.at(1), " label='p1: '");
	TwAddVarRO(bar, "boxPoint2", TW_TYPE_DIR3F, &cubes[0]->m_points.at(2), " label='p2: '");
	TwAddVarRO(bar, "boxPoint3", TW_TYPE_DIR3F, &cubes[0]->m_points.at(3), " label='p3: '");
	TwAddVarRO(bar, "boxPoint4", TW_TYPE_DIR3F, &cubes[0]->m_points.at(4), " label='p4: '");
	TwAddVarRO(bar, "boxPoint5", TW_TYPE_DIR3F, &cubes[0]->m_points.at(5), " label='p5: '");
	TwAddVarRO(bar, "boxPoint6", TW_TYPE_DIR3F, &cubes[0]->m_points.at(6), " label='p6: '");
	TwAddVarRO(bar, "boxPoint7", TW_TYPE_DIR3F, &cubes[0]->m_points.at(7), " label='p7: '");
	TwAddVarRO(bar, "cmass", TW_TYPE_DIR3F, &cubes[0]->centre_of_mass, " label='centre of mass: '");
	TwAddVarRW(bar, "cpos", TW_TYPE_DIR3F, &cubes[0]->m_position, " label='Pos: '");
	TwAddVarRO(bar, "AABBmAX", TW_TYPE_DIR3F, &cubes[0]->AABBmax, " label='AABBmax: '");
	TwAddVarRO(bar, "AABBmin", TW_TYPE_DIR3F, &cubes[0]->AABBmin, " label='AABBmin: '");
	TwAddVarRO(bar, "maxAABBx", TW_TYPE_FLOAT, &cubes[0]->maxAABBx, " label='maxAABBx: '");
	TwAddVarRO(bar, "maxAABBy", TW_TYPE_FLOAT, &cubes[0]->maxAABBy, " label='maxAABBy: '");
	TwAddVarRO(bar, "maxAABBz", TW_TYPE_FLOAT, &cubes[0]->maxAABBz, " label='maxAABBz: '");

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
	if (glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			cubes[i]->SetAngularMomentum(glm::vec3(0.1f,0,0));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			cubes[i]->SetAngularMomentum(glm::vec3(0,0.1f,0));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			cubes[i]->SetAngularMomentum(glm::vec3(0,0,0.1f));
		}
	}

	if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS)
	{

		for (int i=0; i<MAXOBJECT; i++)
		{
			float p1 = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));
			float p2 = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));
			float p3 = -0.5f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5f-(-0.5f))));

			float f1 = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50.0f-(-50.0f))));
			float f2 = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50.0f-(-50.0f))));
			float f3 = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(50.0f-(-50.0f))));

			//cubes[i]->ApplyForce(glm::vec3(p1,p2,p3),glm::vec3(f1,f2,f3));
			//cubes[i]->SetOrientation(cubes[i]->GetOrientation() * glm::quat(glm::vec3(p1,p2,p3)));
			cubes[i]->SetLinearMomentum(12.0f * glm::vec3(p1,p2,p3) * cubes[i]->GetMass());
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

	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET ) == GLFW_PRESS){
		m_physicsLabCamera->enabled = true;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET ) == GLFW_PRESS){
		m_physicsLabCamera->enabled = false;
	}

	if (glfwGetKey(window, GLFW_KEY_PERIOD ) == GLFW_PRESS){
		drawPoints = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SLASH ) == GLFW_PRESS){
		drawPoints = false;
	}

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
		if (cubes[i]->m_position.x < -SPACE)
		{
			cubes[i]->m_linearMomentum.x *= -0.60f;
		}
		if (cubes[i]->m_position.x > SPACE)
		{
			cubes[i]->m_linearMomentum.x *= -0.60f;
		}

		if (cubes[i]->m_position.y < -SPACE)
		{
			cubes[i]->m_linearMomentum.y *= -0.60f;
		}
		if (cubes[i]->m_position.y > SPACE)
		{
			cubes[i]->m_linearMomentum.y *= -0.60f;
		}

		if (cubes[i]->m_position.z < -SPACE)
		{
			cubes[i]->m_linearMomentum.z *= -0.60f;
		}
		if (cubes[i]->m_position.z > SPACE)
		{
			cubes[i]->m_linearMomentum.z *= -0.60f;
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
					((cubes[i]->centre_of_mass.x - cubes[j]->centre_of_mass.x) * (cubes[i]->centre_of_mass.x - cubes[j]->centre_of_mass.x)) + 
					((cubes[i]->centre_of_mass.y - cubes[j]->centre_of_mass.y) * (cubes[i]->centre_of_mass.y - cubes[j]->centre_of_mass.y)) + 
					((cubes[i]->centre_of_mass.z - cubes[j]->centre_of_mass.z) * (cubes[i]->centre_of_mass.z - cubes[j]->centre_of_mass.z))
					);

				dis = glm::abs(dis);

				if (dis < (cubes[i]->distanceFromCentreMessToPoint + cubes[j]->distanceFromCentreMessToPoint))
				{
					boundingSphereCubeBuffers[i]->ChangeColors(boundingSpheresCube[i]->redColors);
					boundingSphereCubeBuffers[j]->ChangeColors(boundingSpheresCube[j]->redColors);

					cubesBuffer[i]->ChangeColors(cubesMesh[i]->redColors);
					cubesBuffer[j]->ChangeColors(cubesMesh[j]->redColors);
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
		cubes[i]->maxAABBx = 0.0f; //scale on x axis (double it when scale)
		cubes[i]->maxAABBy = 0.0f;
		cubes[i]->maxAABBz = 0.0f;
		for (int j=0; j<cubes[i]->m_points.size(); j++)
		{
			glm::vec3 p = cubes[i]->m_points[j];
			if (glm::abs(p.x - cubes[i]->centre_of_mass.x) >= cubes[i]->maxAABBx)
			{
				cubes[i]->maxAABBx = glm::abs(p.x - cubes[i]->centre_of_mass.x);
				if (p.x > cubes[i]->centre_of_mass.x)
				{
					cubes[i]->AABBmax.x = p.x;
					cubes[i]->AABBmin.x = p.x - (cubes[i]->maxAABBx * 2);
				}
				else
				{
					cubes[i]->AABBmin.x = p.x;
					cubes[i]->AABBmax.x = p.x + (cubes[i]->maxAABBx * 2);
				}
			}
			if (glm::abs(p.y - cubes[i]->centre_of_mass.y) >= cubes[i]->maxAABBy)
			{
				cubes[i]->maxAABBy = glm::abs(p.y - cubes[i]->centre_of_mass.y);
				if (p.y > cubes[i]->centre_of_mass.y)
				{
					cubes[i]->AABBmax.y = p.y;
					cubes[i]->AABBmin.y = p.y - (cubes[i]->maxAABBy * 2);
				}
				else
				{
					cubes[i]->AABBmin.y = p.y;
					cubes[i]->AABBmax.y = p.y + (cubes[i]->maxAABBy * 2);
				}
			}
			if (glm::abs(p.z - cubes[i]->centre_of_mass.z) >= cubes[i]->maxAABBz)
			{
				cubes[i]->maxAABBz = glm::abs(p.z - cubes[i]->centre_of_mass.z);
				if (p.z > cubes[i]->centre_of_mass.z)
				{
					cubes[i]->AABBmax.z = p.z;
					cubes[i]->AABBmin.z = p.z - (cubes[i]->maxAABBz * 2);
				}
				else
				{
					cubes[i]->AABBmin.z = p.z;
					cubes[i]->AABBmax.z = p.z + (cubes[i]->maxAABBz * 2);
				}
			}
		}
	}
}

void PhysicsLab_2::computAABBOverLap()
{
	//CheckCollisionBroad
	collidingPair.clear();
	for (int i=0; i<MAXOBJECT; i++)
	{
		for (int j=0; j<MAXOBJECT; j++)
		{
			if (i != j)
			{
				if ( !(cubes[i]->AABBmax.x < cubes[j]->AABBmin.x || cubes[i]->AABBmin.x > cubes[j]->AABBmax.x) && 
					!(cubes[i]->AABBmax.y < cubes[j]->AABBmin.y || cubes[i]->AABBmin.y > cubes[j]->AABBmax.y) &&
					!(cubes[i]->AABBmax.z < cubes[j]->AABBmin.z || cubes[i]->AABBmin.z > cubes[j]->AABBmax.z))
				{
					AABBBuffers[i]->ChangeColors(AABBMeshs[i]->redColors);
					AABBBuffers[j]->ChangeColors(AABBMeshs[j]->redColors);

					cPair t;
					t.ID = i;
					t.collidingWith = j;
					collidingPair.push_back(t);

					//CheckCollisionNarrow
					if (!collidingPair.empty())
					{
						if (CheckCollisionNarrow(*cubes[i],*cubes[j]))
						{
							cubesBuffer[i]->ChangeColors(cubesMesh[i]->redColors);
							cubesBuffer[j]->ChangeColors(cubesMesh[j]->redColors);
							//stopTime = true;

							// 							for (int i=0; i<demoResult.size(); i++)
							// 							{
// 								glm::mat4 tPos = glm::mat4(1);
// 								tPos[3][0] = demoResult[i].collidingPointOnObjectA.x;
// 								tPos[3][1] = demoResult[i].collidingPointOnObjectA.y;
// 								tPos[3][2] = demoResult[i].collidingPointOnObjectA.z;
// 
// 								update(tPos,m_shader->GetProgramID());
// 								draw(collidingPointBuffers[0]->vao, collidingPointMesh[0]->vertices.size());
							// 							}

// 							if (!demoResult.empty())
// 							{
// 								RespondCollision(*cubes[i], *cubes[j], demoResult[0].collidingPointOnObjectA, demoResult[0].collidingPointOnObjectB, demoResult[0].collidingNormal);
// 							}
						}
					}
				}
			}
		}
	}
}

//GJK 
bool PhysicsLab_2::CheckCollisionNarrow(Cube &body1, Cube &body2)
{
	//demoResult.clear();
	std::vector<Simplex> simplex;

	glm::vec3 direction = body1.m_position - body2.m_position;

	simplex.push_back(support(direction, body1, body2));

	direction = -simplex[0].minkowskiDifference;
	int counter = 100;

	while (counter > 0)
	{
		Simplex tempSimplex;
		tempSimplex = support(direction, body1, body2);

		// Last point added was not past the origin in this direction
		if(glm::dot(tempSimplex.minkowskiDifference, direction) < 0)
		{
			return false;
		}
		simplex.push_back(tempSimplex);

		//check intersect
		if (processSimplex(simplex, direction))
		{
			if (simplex.size() == 4)
			{
				glm::vec3 normal = EPA(simplex, body1, body2);
				return true;
			}
		}
		counter--;
	}
}


void PhysicsLab_2::RespondCollision(Cube &body1, Cube &body2, glm::vec3 &cpA, glm::vec3 &cpB, glm::vec3 &n)
{
	glm::vec3 rA = cpA - body1.GetPosition();
	glm::vec3 rB = cpB - body2.GetPosition();

	glm::vec3 J = calculateCollisionImpulse(body1, body2, rA, rB, n, 0.8f) * n;

	body1.SetLinearMomentum(body1.GetLinearMomentum() + J);
	body1.SetAngularMomentum(body1.GetAngularMomentum() + glm::cross(rA, J));

	body2.SetLinearMomentum(body2.GetLinearMomentum() - J);
	body2.SetAngularMomentum(body2.GetAngularMomentum() - glm::cross(rB, J));

}

float PhysicsLab_2::calculateCollisionImpulse(Cube &body1, Cube &body2, glm::vec3 &rA, glm::vec3 &rB, glm::vec3 &n, float e)
{
	glm::vec3 pA = body1.GetLinearVelocity() + glm::cross(body1.GetAngularVelocity(), rA);
	glm::vec3 pB = body2.GetLinearVelocity() + glm::cross(body2.GetAngularVelocity(), rB);
	float relativeV = glm::dot(n, pA - pB);

	if(relativeV < 0.0f)
	{
		float j = (-(1 + e) * relativeV) / 
			(body1.m_massInverse + body2.GetMassInverse() + glm::dot(n, body1.GetInertiaInverse() * glm::cross(rA, n)) + glm::dot(n, body2.GetInertiaInverse() * glm::cross(rB, n)));

		return std::max(0.0f, j);
	}

	return 0.0f;
}


glm::vec3 PhysicsLab_2::EPA(std::vector<Simplex>& simplex, Cube &body1, Cube &body2)
{
	std::vector<Face> faces;

	faces.push_back(Face(0, 1, 2, simplex));
	faces.push_back(Face(0, 1, 3, simplex));
	faces.push_back(Face(0, 2, 3, simplex));
	faces.push_back(Face(1, 2, 3, simplex));

	int counter = 100;
	while (counter > 0)
	{
		Face face = findClosestFace(faces);
		Simplex newPoint = support(face.normal, body1, body2);

		glm::vec3 barycentric = toTriangle(glm::vec3(0), face.v1.minkowskiDifference, face.v2.minkowskiDifference, face.v3.minkowskiDifference);
		glm::vec3 contactPoint1 = barycentric.x * face.v1.pointA + barycentric.y * face.v2.pointA + barycentric.z * face.v3.pointA;
		glm::vec3 contactPoint2 = barycentric.x * face.v1.pointB + barycentric.y * face.v2.pointB + barycentric.z * face.v3.pointB;

		if(glm::dot(newPoint.minkowskiDifference - face.v1.minkowskiDifference, face.normal) - glm::dot(face.v1.minkowskiDifference, face.normal) < 0.01f) 
		{
			glm::vec3 result = -face.normal * glm::dot(-face.v1.minkowskiDifference, -face.normal);

			if(result != glm::vec3())
			{
				DemoResult dr;
				dr.collidingNormal = glm::normalize(result);
				dr.collidingPointOnObjectA = contactPoint1;
				dr.collidingPointOnObjectB = contactPoint2;

				//demoResult.push_back(dr);

				glm::mat4 tPos = glm::mat4(1.0);
				tPos[3][0] = contactPoint1.x;
				tPos[3][1] = contactPoint1.y;
				tPos[3][2] = contactPoint1.z;

				update(tPos,m_shader->GetProgramID());
				draw(collidingPointBuffers[0]->vao, collidingPointMesh[0]->vertices.size());

				//glm::mat4 tPos = glm::mat4();
				tPos[3][0] = contactPoint2.x;
				tPos[3][1] = contactPoint2.y;
				tPos[3][2] = contactPoint2.z;

				update(tPos,m_shader->GetProgramID());
				draw(collidingPointBuffers[1]->vao, collidingPointMesh[1]->vertices.size());

				line->SetFromTo(glm::vec3(result.x * -5, result.y * -5, result.z * -5),glm::vec3(result.x * 5, result.y * 5, result.z * 10));
				update(tPos,b_shader->GetProgramID());
				line->Render(m_shader->GetProgramID());

				RespondCollision(body1, body2, dr.collidingPointOnObjectA, dr.collidingPointOnObjectB, dr.collidingNormal);

				return dr.collidingNormal;
			}
			else
			{
				return glm::vec3(glm::vec3::_null);
			}
		}

		// Add new vertex
		//simplex.erase(simplex.begin());
		simplex.push_back(newPoint);

		// Add relevant faces
		faces.push_back(Face(simplex.size()-1, face.i1, face.i2, simplex));
		faces.push_back(Face(simplex.size()-1, face.i1, face.i3, simplex));
		faces.push_back(Face(simplex.size()-1, face.i2, face.i3, simplex)); 
		counter--;
	}

	return glm::vec3(glm::vec3::_null);
}


glm::vec3 PhysicsLab_2::toTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	// code from Crister Erickson's Real-Time Collision Detection
	glm::vec3 v0 = b - a;
	glm::vec3 v1 = c - a;
	glm::vec3 v2 = p - a;
	float d00 = glm::dot(v0,v0);
	float d01 = glm::dot(v0,v1);
	float d11 = glm::dot(v1,v1);
	float d20 = glm::dot(v2,v0);
	float d21 = glm::dot(v2,v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f -  v -  w;

	glm::vec3 barycentric(u,v,w);

	return barycentric;
}


Face PhysicsLab_2::findClosestFace(std::vector<Face> &faces)
{
	float minDistance = FLT_MAX;
	int minIndex = 0;

	for(int i=0; i<faces.size(); ++i)
	{
		float distance = glm::dot(faces[i].v1.minkowskiDifference, faces[i].normal);

		if(distance < minDistance)
		{
			minDistance = distance;
			minIndex = i;
		}
	}

	Face closest = faces[minIndex];
	faces.erase(faces.begin() + minIndex);

	return closest;
}



glm::vec3 PhysicsLab_2::getFarthestPointInDirection(glm::vec3 direction, const std::vector<glm::vec3>& vertices)
{
	float maxDot = glm::dot(vertices[0],direction);

	int indexDot = 0;
	float currentDot;
	for (int i = 1; i < vertices.size(); i++)
	{
		currentDot = glm::dot(direction,vertices[i]);
		if (currentDot > maxDot){
			maxDot = currentDot;
			indexDot = i;
		}
	}

	return vertices[indexDot];
}

Simplex PhysicsLab_2::support(glm::vec3 direction, Cube &body1, Cube &body2)
{
	Simplex s;
	s.pointA = getFarthestPointInDirection(direction, body1.m_points);
	s.pointB = getFarthestPointInDirection(-direction, body2.m_points);
	s.minkowskiDifference = s.pointA - s.pointB;

	return s;
}

bool PhysicsLab_2::isSameDirection(glm::vec3 &a, glm::vec3 &b)
{
	float dot = glm::dot(a, b);
	return dot > 0.0f;
}

bool PhysicsLab_2::processSimplex(std::vector<Simplex> &simplex, glm::vec3 &direction)
{
	Simplex A,B,C,D;
	glm::vec3 AB,AC,AD,AO;

	switch(simplex.size())
	{
	case 2:

		A = simplex.at(1);
		B = simplex.at(0);

		AB = B.minkowskiDifference - A.minkowskiDifference;
		AO = -A.minkowskiDifference;


		if(isSameDirection(AO,AB))
		{
			direction = glm::cross(glm::cross(AB, AO), AB);
		}
		else
		{
			direction = AO;
		}

		return false;
	case 3:

		//simplex.erase(simplex.begin());

		return checkTriangle(simplex, direction);

		return false;
	case 4:
		A = simplex.at(3);
		B = simplex.at(2);
		C = simplex.at(1);
		D = simplex.at(0);

		AB = B.minkowskiDifference - A.minkowskiDifference;
		AC = C.minkowskiDifference - A.minkowskiDifference;
		AD = D.minkowskiDifference - A.minkowskiDifference;
		AO = -A.minkowskiDifference;

		glm::vec3 ABC = glm::cross(AB, AC);
		glm::vec3 ADB = glm::cross(AD, AB);
		glm::vec3 ACD = glm::cross(AC, AD);

		if(isSameDirection(ABC, AO))
		{			
			simplex.erase(simplex.begin());

			return checkTriangle(simplex, direction);
		}
		else if(isSameDirection(ADB, AO))
		{
			simplex.erase(simplex.begin() + 1);
			simplex[0] = B;
			simplex[1] = D;

			return checkTriangle(simplex, direction);
		}
		else if(isSameDirection(ACD, AO))
		{
			simplex.erase(simplex.begin() + 2);

			return checkTriangle(simplex, direction);
		}

		return true;
	}
}

bool PhysicsLab_2::checkTriangle(std::vector<Simplex> &simplex, glm::vec3 &direction)
{
	Simplex A,B,C;
	glm::vec3 AB,AC,AO;

	A = simplex[2];
	B = simplex[1];
	C = simplex[0];

	AB = B.minkowskiDifference - A.minkowskiDifference;
	AC = C.minkowskiDifference - A.minkowskiDifference;
	AO = -A.minkowskiDifference;

	glm::vec3 ABC = glm::cross(AB, AC);

	if(isSameDirection(glm::cross(ABC, AC), AO)) // AC plane 
	{
		if(isSameDirection(AC, AO)) // outside AC edge
		{
			direction = glm::cross(glm::cross(AC, AO), AC);
			simplex.erase(simplex.begin() + 1);
		}
		else
		{
			if(isSameDirection(AB, AO)) // outside AB edge
			{
				direction = glm::cross(glm::cross(AB, AO), AB);
				simplex.erase(simplex.begin());
			}
			else // outside A
			{
				direction = AO;
				simplex.erase(simplex.begin());
				simplex.erase(simplex.begin());
			}
		}
	}
	else // inside AC 
	{
		if(isSameDirection(glm::cross(AB, ABC), AO)) // AB plane 
		{
			if(isSameDirection(AB, AO)) // outside AB plane
			{
				direction = glm::cross(glm::cross(AB, AO), AB);
				simplex.erase(simplex.begin());
			}
			else // outside A
			{
				direction = AO;
				simplex.erase(simplex.begin());
				simplex.erase(simplex.begin());
			}
		}
		else // orthogonal to face
		{
			if(isSameDirection(ABC, AO)) // outside face
			{
				direction = ABC;
			}
			else // inside face
			{
				simplex[0] = B;
				simplex[1] = C;

				direction = -ABC;
			}
		}
	}
	return false;
}


// based on book "Real-Time Collision Detection" page 336.
void PhysicsLab_2::computAABBOverLapWithSweepAndPrune()
{
	axisX.clear();
	axisY.clear();
	axisZ.clear();
	collidingPair.clear();

	for (int i=0; i<MAXOBJECT; i++)
	{
		AABB t;
		t.ID = i;
		t.max_point = cubes[i]->AABBmax.x;
		t.min_point = cubes[i]->AABBmin.x;
		axisX.push_back(t);

		t.max_point = cubes[i]->AABBmax.y;
		t.min_point = cubes[i]->AABBmin.y;
		axisY.push_back(t);

		t.max_point = cubes[i]->AABBmax.z;
		t.min_point = cubes[i]->AABBmin.z;
		axisZ.push_back(t);
	}

	std::sort(axisX.begin(),axisX.end());
	std::sort(axisY.begin(),axisY.end());
	std::sort(axisZ.begin(),axisZ.end());

	float s[3] = {0,0,0};
	float s2[3] = {0,0,0};
	float v[3];

	for (int i=0; i<MAXOBJECT; i++)
	{
		for (int c=0; c<3; c++)
		{
			s[c] += cubes[i]->m_position[c];
			s2[c] += cubes[i]->m_position[c] * cubes[i]->m_position[c];
		}

		for (int j=0; j<MAXOBJECT; j++)
		{
			if (i != j)
			{
				std::vector<AABB> currentAxis;
				switch (gSortAxis)
				{
				case 0:
					currentAxis = axisX;
					break;
				case 1:
					currentAxis = axisY;
					break;
				case 2:
					currentAxis = axisZ;
					break;
				}

				if (currentAxis[j].min_point > currentAxis[i].max_point)
				{
					break;
				}

				if (AABBOverlap(*cubes[i],*cubes[j]))
				{
					AABBBuffers[i]->ChangeColors(AABBMeshs[i]->redColors);
					AABBBuffers[j]->ChangeColors(AABBMeshs[j]->redColors);

					cPair t;
					t.ID = i;
					t.collidingWith = j;
					collidingPair.push_back(t);

					if (!collidingPair.empty())
					{
						if (CheckCollisionNarrow(*cubes[i],*cubes[j]))
						{
							cubesBuffer[i]->ChangeColors(cubesMesh[i]->redColors);
							cubesBuffer[j]->ChangeColors(cubesMesh[j]->redColors);
						}
					}
				}
			}
		}
	}

	for (int c=0; c<3; c++)
	{
		v[c] = s2[c] - s[c] * s[c] / MAXOBJECT;
	}

	gSortAxis = 0;
	if (v[1] > v[0]) gSortAxis = 1;
	if (v[2] > v[gSortAxis]) gSortAxis = 2;
}


bool PhysicsLab_2::AABBOverlap(const Cube &a, const Cube &b)
{
	// center mass updated every frames 
	// maxAABB is the Radius of bounding box
	if (std::fabs(a.centre_of_mass.x - b.centre_of_mass.x) > (a.maxAABBx + b.maxAABBx )) return false;
	if (std::fabs(a.centre_of_mass.y - b.centre_of_mass.y) > (a.maxAABBy + b.maxAABBy )) return false;
	if (std::fabs(a.centre_of_mass.z - b.centre_of_mass.z) > (a.maxAABBz + b.maxAABBz )) return false;

	// 
	// 	if (a.AABBmax.x < b.AABBmin.x || a.AABBmin.x > b.AABBmax.x) return false;
	// 	if (a.AABBmax.y < b.AABBmin.y || a.AABBmin.y > b.AABBmax.y) return false;
	// 	if (a.AABBmax.z < b.AABBmin.z || a.AABBmin.z > b.AABBmax.z) return false;

	return true;
}
