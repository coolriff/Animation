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
	fingerSpheres = new CreateMesh();
	fingerSphereBuffers = new ObjectBuffer();
	fingerSpheresPos = glm::vec3(2,100,3);
	fingerSpheresMat = glm::mat4(0);
}


PhysicsFinal::~PhysicsFinal(void)
{
	cleanUp();
	leapMotionCleanup();
}


void PhysicsFinal::run(void)
{
	setupGlfwGlew();
	initShaders();
	initTweakBar();
	leapMotionInit();

	cloth = new Cloth(4, 50, 4, 50, glm::vec3(0,50.0f,0));

	fingerSpheres->createBoundingSphereMesh(3.0f, 20);
	fingerSphereBuffers->GenerateVBO(fingerSpheres->vertices,fingerSpheres->colors,fingerSpheres->normals);
	fingerSphereBuffers->LinkBufferToShaderWithNormal();

	do{
		preDraw();

		glUseProgram(m_shader->GetProgramID());

		m_camera->computeMatricesFromInputs(window);

		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");
		m_shader->SetDirectionalLight(glm::vec3(0,0,-1));

		m_camera->handleMVP(modelLoc, viewLoc, projLoc);

		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET ) == GLFW_PRESS){
			m_camera->enabled = true;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET ) == GLFW_PRESS){
			m_camera->enabled = false;
		}

		if (glfwGetKey(window, GLFW_KEY_L ) == GLFW_PRESS){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (glfwGetKey(window, GLFW_KEY_0 ) == GLFW_PRESS){
			cloth->removePins();
		}

		cloth->planeCollision(glm::vec3(0,-12,0));
		cloth->addForce(glm::vec3(0,-0.98f,0));

		if (glfwGetKey(window, GLFW_KEY_7 ) == GLFW_PRESS){
			cloth->windForce(glm::vec3(0.5,0,0.2));
		}

		//cloth->windForce(glm::vec3(0.5,0,0.2));
		cloth->timeStep();
		cloth->ballCollision(fingerSpheresPos, 3.3f);
		cloth->selfCollision();

		if (glfwGetKey(window, GLFW_KEY_8 ) == GLFW_PRESS){
			cloth->selfCollision();
		}

		//cloth->ballTearing(fingerSpheresPos, 3.3f);
		cloth->drawShaded();
		
		for (int i=0; i<cloth->triangles.size(); i++)
		{
			if (cloth->triangles[i].drawable)
			{
				draw(cloth->triangles[i].triangleBuffer->vao, cloth->triangles[i].v.size());
			}
		}
		//draw(cloth->clothBuffer->vao, cloth->v.size());

		leapMotionUpdate();

		fingerSpheresMat = glm::translate(fingerSpheresPos);
		update(fingerSpheresMat, m_shader->GetProgramID());
		draw(fingerSphereBuffers->vao, fingerSpheres->vertices.size());

		TwDraw();
		// Swap buffers
		glfwSwapBuffers(getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(getWindow()) == 0 );
}


void PhysicsFinal::update(glm::mat4 ModelMatrix, GLuint shaderProgramID)
{
	GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
}

void PhysicsFinal::draw(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, size);
	glBindVertexArray(0);
}

void PhysicsFinal::drawLine(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, size);
	glBindVertexArray(0);
}


void PhysicsFinal::initShaders()
{
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("BlinnPhong.vs",vertexShaderSourceCode);
	m_shader->readShaderFile("BlinnPhong.ps",fragmentShaderSourceCode);
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
	//glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.2f, 0.2f, 0.0f);

	// 	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
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
 	TwAddVarRW(bar, "fingerSpheresPos", TW_TYPE_DIR3F, &fingerSpheresPos, " label='fingerSpheresPos: '");
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

bool PhysicsFinal::leapMotionInit(void)
{
	if (leapMotionController.addListener(leapMotionListener))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PhysicsFinal::leapMotionUpdate(void)
{
	if(leapMotionController.isConnected())
	{
		Leap::Frame leapFrameData = leapMotionController.frame();
		Leap::Hand leapHand = leapFrameData.hands().rightmost();

		Leap::FingerList fingerListNow = leapFrameData.hands()[0].fingers();
		std::vector<glm::vec3> fingerPositions;

		//printf("ye!!");

		for(int i = 0; i < fingerListNow.count(); i++)
		{
			Leap::Vector fp = fingerListNow[i].tipPosition();
			fingerPositions.push_back(glm::vec3(fp.x, fp.y, fp.z));
		}
 
		if(fingerPositions.size() > 0)
		{
			//printf("yet");
			//finger 1
			Leap::Finger finger0 = leapHand.fingers()[0];
			Leap::Vector fp0 = finger0.tipPosition();

			fingerSpheresPos = glm::vec3(fp0.x * 0.2, (fp0.y-110) * 0.2, fp0.z * 0.2);
		}


		if(leapFrameData.hands().isEmpty() )
		{
// 			fingerSpheresPos = glm::vec3(2,-2,3);
// 			fingerSpheresMat = glm::translate(fingerSpheresPos);
// 			update(fingerSpheresMat, m_shader->GetProgramID());
// 			draw(fingerSphereBuffers->vao, fingerSpheres->vertices.size());
			//printf("emputy");
		}

 	}
}

void PhysicsFinal::leapMotionCleanup(void)
{
	leapMotionController.removeListener(leapMotionListener);
}

