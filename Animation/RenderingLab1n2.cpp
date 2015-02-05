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
	tooShader = false;
	stdShader = true;
	bShader = false;
	MMShader = false;
	shaderType = STANDARD;
	isTexture = true;
	shader = new Shader();
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

// 	modelsMesh[0].LoadMesh("../Models/teapot2.obj");
// 	modelsMesh[1].LoadMesh("../Models/teapot2.obj");
// 
// 	for (int i=0; i<MAXOBJECT; i++)
// 	{
// 		modelsBuffer[i].GenerateVBO(modelsMesh[i].vertices, modelsMesh[i].colors, modelsMesh[i].normals);
// 		models[i].ID = i;
// 		models[i].m_orientation = glm::quat();
// 	}
// 
// 	for (int i=0; i<MAXOBJECT; i++)
// 	{
// 		for (int j=0; j<MAXSHADERTYPE; j++)
// 		{
// 			modelsBuffer[i].LinkBufferToShaderWithNormal(m_shader[j].GetProgramID());
// 			modelsBuffer[i].SetTexture("../Models/bricks.jpg",m_shader[j].GetProgramID());
// 		}
// 	}
// 
// 
// 	models[0].m_position = glm::vec3(-3,0,0);
// 	models[1].m_position = glm::vec3(3,0,0);

// 	m_body.m_position = glm::vec3(0);
// 	m_bodyMesh.LoadMesh("../Models/teapot2.obj");
// 	m_bodyBuffer.tony(m_bodyMesh.vertices, m_bodyMesh.colors, m_bodyMesh.normals,m_bodyMesh.texcoords, m_bodyMesh.indices);

	//m_bodyBuffer.LinkBufferToShaderWithNormal(m_shader[0].GetProgramID());
	//m_bodyBuffer.SetTexture("../Models/bricks.jpg",m_shader[0].GetProgramID());

	//m_bodyBuffer.LinkBufferToShaderWithNormal(m_shaderTexture[0].GetProgramID());
	//m_bodyBuffer.SetTexture("../Models/gold.png", shader->GetProgramID());


	teapot = new MeshLoader(shader->GetProgramID(), "../Models/teapot2.obj");
	teapot->SetPos(glm::vec3(3,0,0));
	teapot->SetScale(glm::vec3(0.8,0.8,0.8));
	teapot->SetPossibleShaders(shader->GetProgramID());
	teapot->SetColor(glm::vec3(1,1,1));
	teapot->SetTexture("../Models/gold.png");




	directionalLightDirection = glm::vec3(0, 0, -1);


	do{
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		if (stopTime)
		{
			delta = 0;
		}

		preDraw();
		
		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET ) == GLFW_PRESS){
			isTexture = true;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET ) == GLFW_PRESS){
			isTexture = false;
		}

		if (glfwGetKey(window, GLFW_KEY_7 ) == GLFW_PRESS){
			shaderType = STANDARD;
			stdShader = true;
			tooShader = false;
			bShader = false;
			MMShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
			shaderType = CARTOON;
			stdShader = false;
			tooShader = true;
			bShader = false;
			MMShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_9 ) == GLFW_PRESS){
			shaderType = WHATEVER;
			stdShader = false;
			tooShader = false;
			bShader = true;
			MMShader = false;
		}
		if (glfwGetKey(window, GLFW_KEY_0 ) == GLFW_PRESS){
			shaderType = NUMBER4;
			stdShader = false;
			tooShader = false;
			bShader = false;
			MMShader = true;
		}


		switch (shaderType)
		{
		case RenderingLab1n2::STANDARD:
// 			if (isTexture)
// 			{
// 				glUseProgram(m_shaderTexture[0].GetProgramID());
// 			}
// 			else
// 			{
// 				glUseProgram(m_shader[0].GetProgramID());
// 			}
 			break;
		case RenderingLab1n2::CARTOON:
			glUseProgram(m_shader[1].GetProgramID());
			break;
		case RenderingLab1n2::WHATEVER:
			glUseProgram(m_shader[2].GetProgramID());
			break;
		case RenderingLab1n2::NUMBER4:
			//glUseProgram(cylinderShader->GetProgramID());
			break;
		}

		keyControl();
// 		for (int i=0; i<MAXOBJECT; i++)
// 		{
// 			models[i].Update(delta);
// 		}
		
		m_body.Update(delta);

				glUseProgram(shader->GetProgramID());
				m_physicsLabCamera->computeMatricesFromInputs(window);
				GLuint modelLoc = glGetUniformLocation(shader->GetProgramID(), "model");
				GLuint viewLoc = glGetUniformLocation(shader->GetProgramID(), "view");
				GLuint projLoc = glGetUniformLocation(shader->GetProgramID(), "projection");
				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

		teapot->UpdateShader();
		teapot->Render();

		//ball detecction
// 		if (shaderType == STANDARD)
// 		{
// 			if (isTexture)
// 			{
// 				glUseProgram(shader->GetProgramID());
// 				m_physicsLabCamera->computeMatricesFromInputs(window);
// 				GLuint modelLoc = glGetUniformLocation(shader->GetProgramID(), "model");
// 				GLuint viewLoc = glGetUniformLocation(shader->GetProgramID(), "view");
// 				GLuint projLoc = glGetUniformLocation(shader->GetProgramID(), "projection");
// 				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
// 
// 				//m_shaderTexture[0].SetDirectionalLight(directionalLightDirection);
// 				update(m_body.GetTransformationMatrix(), shader->GetProgramID());
// 
// 
// 
// 				m_bodyBuffer.textureLoader->Bind(GL_TEXTURE0);
// 
// 			
// 				glBindVertexArray(m_bodyBuffer.vao);
// 				//glDrawArrays(GL_TRIANGLES, 0, m_bodyMesh.vertices.size());
// 				glDrawElements(GL_TRIANGLES, m_bodyMesh.numElements, GL_UNSIGNED_INT, NULL);
// 				glBindVertexArray(0);
// 			}
// 			else
// 			{
// 				m_physicsLabCamera->computeMatricesFromInputs(window);
// 				GLuint modelLoc = glGetUniformLocation(m_shader[0].GetProgramID(), "model");
// 				GLuint viewLoc = glGetUniformLocation(m_shader[0].GetProgramID(), "view");
// 				GLuint projLoc = glGetUniformLocation(m_shader[0].GetProgramID(), "projection");
// 				m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);
// 
// 				//m_shader[0].SetDirectionalLight(directionalLightDirection);
// 				update(m_body.GetTransformationMatrix(),m_shader[0].GetProgramID());
// 
// 				draw(m_bodyBuffer.vao, m_bodyMesh.vertices.size()); 
// 			}

			


// 			for (int i=0; i<MAXOBJECT; i++)
// 			{
// 				update(models[i].GetTransformationMatrix(),m_shader[0].GetProgramID());
// 				draw(modelsBuffer[i].vao, modelsMesh[i].vertices.size()); 
// 			}
//		}

		//AABB with SWEEP AND PRUNE
		if (shaderType == CARTOON)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(m_shader[1].GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(m_shader[1].GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(m_shader[1].GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			m_shader[1].SetDirectionalLight(directionalLightDirection);

// 			for (int i=0; i<MAXOBJECT; i++)
// 			{
// 				update(models[i].GetTransformationMatrix(),m_shader[1].GetProgramID());
// 				draw(modelsBuffer[i].vao, modelsMesh[i].vertices.size()); 
// 			}
		}

		//AABB without SWEEP AND PRUNE
		if (shaderType == WHATEVER)
		{
			m_physicsLabCamera->computeMatricesFromInputs(window);
			GLuint modelLoc = glGetUniformLocation(m_shader[2].GetProgramID(), "model");
			GLuint viewLoc = glGetUniformLocation(m_shader[2].GetProgramID(), "view");
			GLuint projLoc = glGetUniformLocation(m_shader[2].GetProgramID(), "projection");
			m_physicsLabCamera->handleMVP(modelLoc, viewLoc, projLoc);

			m_shader[2].SetDirectionalLight(directionalLightDirection);

// 			for (int i=0; i<MAXOBJECT; i++)
// 			{
// 				update(models[i].GetTransformationMatrix(),m_shader[2].GetProgramID());
// 				draw(modelsBuffer[i].vao, modelsMesh[i].vertices.size()); 
// 			}
		}

		if (shaderType == NUMBER4)
		{

		}

		//check distance and change colors

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
	//with texture
	std::string vertexShaderSourceCode_T1,fragmentShaderSourceCode_T1;
	m_shaderTexture[0].readShaderFile("defaultTexture.vs",vertexShaderSourceCode_T1);
	m_shaderTexture[0].readShaderFile("defaultTexture.ps",fragmentShaderSourceCode_T1);
	GLuint vertexShaderID_T1 = m_shaderTexture[0].makeShader(vertexShaderSourceCode_T1.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID_T1 = m_shaderTexture[0].makeShader(fragmentShaderSourceCode_T1.c_str(), GL_FRAGMENT_SHADER);
	m_shaderTexture[0].makeShaderProgram(vertexShaderID_T1,fragmentShaderID_T1);
	printf("vertexShaderID is %d\n",vertexShaderID_T1);
	printf("fragmentShaderID is %d\n",fragmentShaderID_T1);
	printf("shaderProgramID is %d\n",m_shaderTexture[0].GetProgramID());
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


	TwAddVarRO(bar, "std", TW_TYPE_BOOL8, &stdShader, " label='Default(7) '");
	TwAddVarRO(bar, "Too", TW_TYPE_BOOL8, &tooShader, " label='Cartoon(8) '");
	TwAddVarRO(bar, "bs", TW_TYPE_BOOL8, &bShader, " label='BlinnPhong(9) '");
	TwAddVarRO(bar, "mm", TW_TYPE_BOOL8, &MMShader, " label='MM(0) '");


	// 	TwAddVarRO(bar, "boxPoint0", TW_TYPE_DIR3F, &cubes[0]->m_points.at(0), " label='p0: '");
	// 	TwAddVarRO(bar, "boxPoint1", TW_TYPE_DIR3F, &cubes[0]->m_points.at(1), " label='p1: '");
	// 	TwAddVarRO(bar, "boxPoint2", TW_TYPE_DIR3F, &cubes[0]->m_points.at(2), " label='p2: '");
	// 	TwAddVarRO(bar, "boxPoint3", TW_TYPE_DIR3F, &cubes[0]->m_points.at(3), " label='p3: '");
	// 	TwAddVarRO(bar, "boxPoint4", TW_TYPE_DIR3F, &cubes[0]->m_points.at(4), " label='p4: '");
	// 	TwAddVarRO(bar, "boxPoint5", TW_TYPE_DIR3F, &cubes[0]->m_points.at(5), " label='p5: '");
	// 	TwAddVarRO(bar, "boxPoint6", TW_TYPE_DIR3F, &cubes[0]->m_points.at(6), " label='p6: '");
	// 	TwAddVarRO(bar, "boxPoint7", TW_TYPE_DIR3F, &cubes[0]->m_points.at(7), " label='p7: '");
	// 	TwAddVarRO(bar, "cmass", TW_TYPE_DIR3F, &cube->centre_of_mess, " label='centre of mess: '");
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
		for (int i=0; i<MAXOBJECT; i++)
		{
			//models[i].SetAngularMomentum(glm::vec3(0.9f,0,0));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			//models[i].SetAngularMomentum(glm::vec3(0,0.9f,0));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T ) == GLFW_PRESS){
		for (int i=0; i<MAXOBJECT; i++)
		{
			//models[i].SetAngularMomentum(glm::vec3(0,0,0.9f));
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

void RenderingLab1n2::draw(GLuint vao, int size)
{
// 	if(isTexture)
// 	{
// 		m_bodyBuffer.texture->Bind(GL_TEXTURE0);
// 	}
// 	else
// 	{
// 		m_bodyBuffer.texture->UnBind();
// 	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, size);
	//glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void RenderingLab1n2::drawLine(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, size);
	glBindVertexArray(0);
}