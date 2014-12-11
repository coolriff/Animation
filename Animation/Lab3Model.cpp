#include "Lab3Model.h"


Lab3Model::Lab3Model(void)
{
	m_setup = new Setup();
	m_shader = new Shader();
	m_shader_loader = new Shader();
	m_objectBuffer = new ObjectBuffer(36);
	m_camera = new Camera(m_setup);
	point_count = 0;
	humanSkeleton = new HumanSkeleton(m_setup);
	plane = new Cylinder();
	armSkeleton = new ArmSkeleton(m_setup);
	thirdCamera = false;
	thirdCamera_2 = false;
	followBallCamera = false;
	drawCircleCamera = false;
	animationState = false;

	m_mesh = new Mesh();
	m_effect = new SkinningTechnique();

	floor_mesh = new Mesh();
	floor_effect = new SkinningTechnique();

	football_mesh = new Mesh();
	football_effect = new SkinningTechnique();

	charRot = 0.0f;

	startTime = GetCurrentTimeMillis();

	targetPos = glm::vec3(0,0,0);
	conterAni = 0.0f;

	timeFlag = false;
	deltaTime = 0.0f;
	cameraPath = true;
	camTime = 0.01f;
}


Lab3Model::~Lab3Model(void)
{
}

void Lab3Model::run(void)
{
	m_setup->setupGlfwGlew();

	//initShaders();
	initLoaderShaders();


	if (!m_effect->Init(m_shader_loader->GetProgramID())) 
	{
		printf("Error initializing the lighting technique\n");
	}
	m_effect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

	std::string mainName = "../Models/soccer_pass_1/soccer_pass_1.dae";

	if (!m_mesh->LoadMesh(mainName)) 
	{
		cout << "Main Character Load Failed" << endl;
	}

	//floor
	if (!floor_effect->Init(m_shader_loader->GetProgramID())) 
	{
		printf("Error initializing the lighting technique\n");
	}
	floor_effect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

	std::string floorName = "../Models/untitled.dae";

	if (!floor_mesh->LoadMesh(floorName)) 
	{
		cout << "Main Character Load Failed" << endl;
	}

	//football
	if (!football_effect->Init(m_shader_loader->GetProgramID())) 
	{
		printf("Error initializing the lighting technique\n");
	}
	football_effect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

	std::string footName = "../Models/walking_6/walking_6.dae";

	if (!football_mesh->LoadMesh(footName)) 
	{
		cout << "Main Character Load Failed" << endl;
	}

	humanSkeleton->createHumanNode();
	humanSkeleton->drawHumanMesh(m_shader_loader->GetProgramID());

	humanSkeleton->createArmNode();
	humanSkeleton->drawArmMesh(m_shader_loader->GetProgramID());

	createGate();

	plane = new Cylinder(100, 50, 50, glm::vec4(0.2, 0.6, 0.3, 1.0), glm::vec4(0.2, 0.6, 0.3, 1.0), 2);
	plane->generateObjectBuffer(m_shader_loader->GetProgramID());

	stag[0] = new Cylinder(3, 0.5, 0.5, glm::vec4(0.2, 0.6, 0.3, 1.0), glm::vec4(0.2, 0.6, 0.3, 1.0), 2);
	stag[0]->generateObjectBuffer(m_shader_loader->GetProgramID());

	stag[1] = new Cylinder(2, 1, 0, glm::vec4(0.2, 0.6, 0.3, 1.0), glm::vec4(0.2, 0.6, 0.3, 1.0), 2);
	stag[1]->generateObjectBuffer(m_shader_loader->GetProgramID());
	

	do{
		m_setup->preDraw();

		//glUseProgram(m_shader->GetProgramID());
		glUseProgram(m_shader_loader->GetProgramID());

		armSkeleton->createArmNode();
		armSkeleton->drawArmMesh(m_shader_loader->GetProgramID());

		m_camera->computeMatricesFromInputs();

// 		GLuint modelLoc = glGetUniformLocation(m_shader->GetProgramID(), "model");
// 		GLuint viewLoc = glGetUniformLocation(m_shader->GetProgramID(), "view");
// 		GLuint projLoc = glGetUniformLocation(m_shader->GetProgramID(), "projection");

		GLuint modelLoaderLoc = glGetUniformLocation(m_shader_loader->GetProgramID(), "model");
		GLuint viewLoaderLoc = glGetUniformLocation(m_shader_loader->GetProgramID(), "view");
		GLuint projLoaderLoc = glGetUniformLocation(m_shader_loader->GetProgramID(), "projection");

// 		vector<Matrix4f> Transforms;
// 
// 		glm::mat4 animationMan;
// 
// 		if (animationState)
// 		{
// 			bool tm = false;
// 			float RunningTime = GetRunningTime();
// 
// 			m_mesh->BoneTransform(RunningTime,Transforms);
// 
// 			for (GLuint i = 0 ; i < Transforms.size() ; i++) 
// 			{
// 				m_effect->SetBoneTransform(i, Transforms[i]);
// 
// 				if (i+1 == Transforms.size() )
// 				{
// 					tm = true;
// 				}
// 			}
// 			animationMan = glm::translate(animationMan,glm::vec3(50,0,20));
// 			animationMan = glm::rotate(convertAssimpMatrix(m_mesh->m_GlobalInverseTransform), 180.0f, glm::vec3(0,1,0));
// 			animationMan = glm::scale(animationMan,glm::vec3(0.05f,0.05f,0.05f));
// 			m_mesh->Update(modelLoaderLoc,animationMan);
// 			m_mesh->Render();
// 
// 			if (tm)
// 			{
// 				animationState = false;
// 			}
// 		}
// 		else
// 		{
// 			animationMan = glm::translate(animationMan,glm::vec3(50,0,20));
// 			animationMan = glm::rotate(convertAssimpMatrix(m_mesh->m_GlobalInverseTransform),180.0f, glm::vec3(0,1,0));
// 			animationMan = glm::scale(animationMan,glm::vec3(0.05f,0.05f,0.05f));
// 			m_mesh->Update(modelLoaderLoc,animationMan);
// 			m_mesh->Render();
// 		}
		glm::mat4 animationMat;

		vector<Matrix4f> Transforms;	

		if (thirdCamera_2)
		{
			if(glfwGetKey(m_setup->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			{
				targetPos.z -=0.1;
				animationMat = glm::translate(animationMat,targetPos);
				animationMat = glm::rotate(animationMat,180.0f, glm::vec3(0,1,0));
				animationMat = glm::scale(animationMat,glm::vec3(0.05f,0.05f,0.05f));
				m_mesh->Update(modelLoaderLoc,animationMat);
				m_mesh->Render();
			}
			if(glfwGetKey(m_setup->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			{
				targetPos.z +=0.1;
				animationMat = glm::translate(animationMat,targetPos);
				animationMat = glm::rotate(animationMat,180.0f, glm::vec3(0,1,0));
				animationMat = glm::scale(animationMat,glm::vec3(0.05f,0.05f,0.05f));
				m_mesh->Update(modelLoaderLoc,animationMat);
				m_mesh->Render();
			}
			if(glfwGetKey(m_setup->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			{
				targetPos.x -=0.1;
				animationMat = glm::translate(animationMat,targetPos);
				animationMat = glm::rotate(animationMat,180.0f, glm::vec3(0,1,0));
				animationMat = glm::scale(animationMat,glm::vec3(0.05f,0.05f,0.05f));
				m_mesh->Update(modelLoaderLoc,animationMat);
				m_mesh->Render();
			}
			if(glfwGetKey(m_setup->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			{
				targetPos.x +=0.1;
				animationMat = glm::translate(animationMat,targetPos);
				animationMat = glm::rotate(animationMat,180.0f, glm::vec3(0,1,0));
				animationMat = glm::scale(animationMat,glm::vec3(0.05f,0.05f,0.05f));
				m_mesh->Update(modelLoaderLoc,animationMat);
				m_mesh->Render();
			}
		}
		

		if (humanSkeleton->hAnimation)
		{
			float RunningTime = this->GetRunningTime();
			m_mesh->BoneTransform(RunningTime,Transforms);

			for (GLuint i = 0 ; i < Transforms.size() ; i++) 
			{
				m_effect->SetBoneTransform(i, Transforms[i]);
			}

			conterAni += 0.01f;
			if (conterAni > 0.75f)
			{
				humanSkeleton->hAnimation = false;
				conterAni = 0.0f;
			}
		}
		else
		{
			m_mesh->BoneTransform(0,Transforms);

			for (GLuint i = 0 ; i < Transforms.size() ; i++) 
			{
				m_effect->SetBoneTransform(i, Transforms[i]);
			}	
		}

		animationMat = glm::translate(animationMat,targetPos);
		animationMat = glm::rotate(animationMat,180.0f, glm::vec3(0,1,0));
		animationMat = glm::scale(animationMat,glm::vec3(0.05f,0.05f,0.05f));

		m_mesh->Update(modelLoaderLoc,animationMat);
		m_mesh->Render();

		humanSkeleton->hModelMat = animationMat;

		//floor
		glm::mat4 floors = glm::translate(floors,glm::vec3(0,0,-50));
		floors = glm::rotate(glm::mat4(1), 90.0f, glm::vec3(-1,0,0));
		floors = glm::scale(floors,glm::vec3(100.0f,100.0f,100.0f));
		floor_mesh->Update(modelLoaderLoc,floors);
		floor_mesh->Render();

		//football
		glm::mat4 mans = glm::translate(glm::mat4(1),glm::vec3(0,-1000,0));
		mans = glm::scale(mans,glm::vec3(0.01f,0.01f,0.01f));
		football_mesh->Update(modelLoaderLoc,mans);
		football_mesh->Render();

		timeKeyControl();

		if (cameraPath)
		{	
			camTime+=0.01f;
			if(camTime >= 3)
			{
				camTime = 0;
				cameraPath = false;
			}
			pathPos = humanSkeleton->interpolateCubic(camTime,glm::vec3(-100,50,-100),glm::vec3(100,50,-100),
				glm::vec3(100,50,100),glm::vec3(-100,50,100));
			m_camera->cameraUpdate(pathPos, glm::vec3(0,20,0));
		}

		m_camera->handleMVP(modelLoaderLoc, viewLoaderLoc, projLoaderLoc);



		drawGate();
		staging();

		humanSkeleton->keyControl(m_shader_loader->GetProgramID());
		humanSkeleton->updateBallPos(m_shader_loader->GetProgramID());
		humanSkeleton->calculateInverseKinematics();
		humanSkeleton->updateStar(m_shader_loader->GetProgramID());
		
		humanSkeleton->calcGlobalTransformation();
		humanSkeleton->updateHumanMesh(m_shader_loader->GetProgramID());
		humanSkeleton->updateArmMesh(m_shader_loader->GetProgramID());

// 		armSkeleton->updateArmTarget(m_shader->GetProgramID());
// 		armSkeleton->calculateInverseKinematics();
 		humanSkeleton->updateArmMesh(m_shader_loader->GetProgramID());

// 		glUseProgram(m_shader->GetProgramID());
// 		glBindVertexArray(vao);
// 		glDrawArrays(GL_TRIANGLES, 0, point_count);

		// Swap buffers
		glfwSwapBuffers(m_setup->getWindow());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(m_setup->getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(m_setup->getWindow()) == 0 );
}


void Lab3Model::initShaders()
{
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader->readShaderFile("skinning.vs",vertexShaderSourceCode);
	m_shader->readShaderFile("diffuse.ps",fragmentShaderSourceCode);
	GLuint vertexShaderID = m_shader->makeShader(vertexShaderSourceCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = m_shader->makeShader(fragmentShaderSourceCode.c_str(), GL_FRAGMENT_SHADER);
	m_shader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",m_shader->GetProgramID());
}

glm::mat4 Lab3Model::convertAssimpMatrix (Matrix4f m) 
{
	glm::mat4 to;

	to[0][0] = m.m[0][0];	to[0][1] = m.m[1][0];	to[0][2] = m.m[2][0];	to[0][3] = m.m[3][0];
	to[1][0] = m.m[0][1];	to[1][1] = m.m[1][1];	to[1][2] = m.m[2][1];	to[1][3] = m.m[3][1];
	to[2][0] = m.m[0][2];	to[2][1] = m.m[1][2];	to[2][2] = m.m[2][2];	to[2][3] = m.m[3][2];
	to[3][0] = m.m[0][3];	to[3][1] = m.m[1][3];	to[3][2] = m.m[2][3];	to[3][3] = m.m[3][3];

	return to;
}

void Lab3Model::drawGate()
{
	glm::mat4 gateMat40 = glm::translate(glm::rotate(glm::mat4(1), 90.0f, glm::vec3(0,0,1)),glm::vec3(10,0,-100));
	//gateMat40 = glm::rotate(glm::mat4(1), 90.0f, glm::vec3(0,0,1));
	gate[0]->update(gateMat40, m_shader_loader->GetProgramID());
	gate[0]->draw();

	glm::mat4 gateMat41 = glm::translate(glm::rotate(glm::mat4(1), -90.0f, glm::vec3(0,0,1)),glm::vec3(-10,0,-100));
	//gateMat40 = glm::rotate(glm::mat4(1), 90.0f, glm::vec3(0,0,1));
	gate[1]->update(gateMat41, m_shader_loader->GetProgramID());
	gate[1]->draw();

	glm::mat4 gateMat42 = glm::translate(glm::mat4(1),glm::vec3(-15,0,-100));
	gate[2]->update(gateMat42, m_shader_loader->GetProgramID());
	gate[2]->draw();

	glm::mat4 gateMat43 = glm::translate(glm::mat4(1),glm::vec3(15,0,-100));
	gate[3]->update(gateMat43, m_shader_loader->GetProgramID());
	gate[3]->draw();

	glm::mat4 gateMat44 = glm::translate(glm::mat4(1),glm::vec3(-15,10,-100));
	glm::mat4 offset0 = gateMat44;
	gateMat44 = glm::rotate(offset0, -90.0f, glm::vec3(1,0,0));
	gate[4]->update(gateMat44, m_shader_loader->GetProgramID());
	gate[4]->draw();

	glm::mat4 gateMat45 = glm::translate(glm::mat4(1),glm::vec3(15,10,-100));
	glm::mat4 offset1 = gateMat45;
	gateMat45 = glm::rotate(offset1, -90.0f, glm::vec3(1,0,0));
	gate[5]->update(gateMat45, m_shader_loader->GetProgramID());
	gate[5]->draw();

	glm::mat4 gateMat46 = glm::translate(glm::mat4(1),glm::vec3(15,0,-110));
	glm::mat4 offset2 = gateMat46;
	gateMat46 = glm::rotate(offset2, 27.0f, glm::vec3(1,0,0));
	gate[6]->update(gateMat46, m_shader_loader->GetProgramID());
	gate[6]->draw();

	glm::mat4 gateMat47 = glm::translate(glm::mat4(1),glm::vec3(-15,0,-110));
	glm::mat4 offset3 = gateMat47;
	gateMat47 = glm::rotate(offset3, 27.0f, glm::vec3(1,0,0));
	gate[7]->update(gateMat47, m_shader_loader->GetProgramID());
	gate[7]->draw();
}

void Lab3Model::createGate()
{
	gate[0] = new Cylinder(15, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[0]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[1] = new Cylinder(15, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[1]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[2] = new Cylinder(10, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[2]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[3] = new Cylinder(10, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[3]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[4] = new Cylinder(5, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[4]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[5] = new Cylinder(5, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[5]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[6] = new Cylinder(11.5, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[6]->generateObjectBuffer(m_shader_loader->GetProgramID());

	gate[7] = new Cylinder(11.5, 0.4, 0.4, glm::vec4(1.0, 0.1, 0.1, 1.0), glm::vec4(0.1, 0.1, 1.0, 1.0), 16);
	gate[7]->generateObjectBuffer(m_shader_loader->GetProgramID());
}

void Lab3Model::timeKeyControl()
{
	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_R ) == GLFW_PRESS){
		thirdCamera = true;
		thirdCamera_2 = false;
		humanSkeleton->cModel = true;
		humanSkeleton->hModel = false;
// 		humanSkeleton->cball = true;
// 		humanSkeleton->hball = false;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_V ) == GLFW_PRESS){
		thirdCamera_2 = true;
		thirdCamera = false;
		humanSkeleton->hModel = true;
		humanSkeleton->cModel = false;
// 		humanSkeleton->cball = false;
// 		humanSkeleton->hball = true;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_G ) == GLFW_PRESS){
		thirdCamera_2 = false;
		thirdCamera = false;
		followBallCamera = true;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_V ) == GLFW_PRESS){
		thirdCamera_2 = true;
		thirdCamera = false;
		humanSkeleton->hModel = true;
		humanSkeleton->cModel = false;
		// 		humanSkeleton->cball = false;
		// 		humanSkeleton->hball = true;
	}

	if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_F ) == GLFW_PRESS)
	{
		thirdCamera = false;
		thirdCamera_2 = false;
		followBallCamera = false;
		//m_camera->setPosition(humanSkeleton->slerp(m_camera->position,glm::vec3(0,10,-20),0.1));
	}

	if (thirdCamera)
	{
		thirdCamera_2 = false;
		followBallCamera = false;
		// Move forward
		if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_W ) == GLFW_PRESS){
			if (!humanSkeleton->kick)
			{
				humanSkeleton->humanNode[0]->localTransformation[3][2] -= 0.05;
				humanSkeleton->walking = true;
			}
		}
		// Move backward
		if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_S ) == GLFW_PRESS){
			if (!humanSkeleton->kick)
			{
				humanSkeleton->humanNode[0]->localTransformation[3][2] += 0.05;
				humanSkeleton->walking = true;
			}
		}
		// Strafe right
		if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_D ) == GLFW_PRESS){
			if (!humanSkeleton->kick)
			{
// 				charRot += 0.5f;
// 				glm::mat4 rots = glm::rotate(humanSkeleton->humanNode[0]->offset, charRot, m_camera->up);
// 				humanSkeleton->humanNode[0]->localTransformation = rots;
// 				m_camera->ViewMatrix = rots;
				humanSkeleton->humanNode[0]->localTransformation[3][0] += 0.05;
				humanSkeleton->walking = true;
			}
		}
		// Strafe left
		if (glfwGetKey( m_setup->getWindow(), GLFW_KEY_A ) == GLFW_PRESS){
			if (!humanSkeleton->kick)
			{
// 				charRot -= 0.5f;
// 				glm::mat4 rots = glm::rotate(humanSkeleton->humanNode[0]->offset, charRot, m_camera->up);
// 				humanSkeleton->humanNode[0]->localTransformation = rots;
// 				m_camera->ViewMatrix = rots;
				humanSkeleton->humanNode[0]->localTransformation[3][0] -= 0.05;
				humanSkeleton->walking = true;
			}
		}

		//humanSkeleton->walking = false;
		//humanSkeleton->humanNode[0]->localTransformation = glm::lookAt(humanSkeleton->humanNode[0]->pos,humanSkeleton->humanNode[0]->pos+m_camera->direction,m_camera->up);
		//m_camera->cameraUpdate(glm::vec3(humanSkeleton->humanNode[0]->pos.x, humanSkeleton->humanNode[0]->pos.y+3.0f, humanSkeleton->humanNode[0]->pos.z + 15.0f), humanSkeleton->humanNode[0]->pos);
		m_camera->cameraUpdate(glm::vec3(humanSkeleton->humanNode[0]->pos.x, humanSkeleton->humanNode[0]->pos.y+3.0f, humanSkeleton->humanNode[0]->pos.z + 15.0f), 
			glm::vec3(humanSkeleton->humanNode[0]->globalTransformation[3][0], humanSkeleton->humanNode[0]->globalTransformation[3][1], humanSkeleton->humanNode[0]->globalTransformation[3][2]));
	}

	if (thirdCamera_2)
	{
		thirdCamera = false;
		followBallCamera = false;


		glm::mat4 temp = humanSkeleton->hModelMat;
		m_camera->cameraUpdate(glm::vec3(temp[3][0]-3, temp[3][1]+10.0f, temp[3][2]+18.0f), glm::vec3(temp[3][0], temp[3][1]+6.0f, temp[3][2]));
	}

	if (followBallCamera)
	{
		thirdCamera = false;
		thirdCamera_2 = false;
		//humanSkeleton->humanNode[0]->localTransformation = glm::lookAt(humanSkeleton->humanNode[0]->pos,humanSkeleton->humanNode[0]->pos+m_camera->direction,m_camera->up);
		m_camera->cameraUpdate(glm::vec3(humanSkeleton->armTargetPos.x, humanSkeleton->armTargetPos.y+3.0f, humanSkeleton->armTargetPos.z + 15.0f), humanSkeleton->armTargetPos);
	}
}

void Lab3Model::initLoaderShaders()
{
	std::string vertexShaderSourceCode,fragmentShaderSourceCode;
	m_shader_loader->readShaderFile("skinning.vs",vertexShaderSourceCode);
	m_shader_loader->readShaderFile("skinning.fs",fragmentShaderSourceCode);
	GLuint vertexShaderID = m_shader_loader->makeShader(vertexShaderSourceCode.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShaderID = m_shader_loader->makeShader(fragmentShaderSourceCode.c_str(), GL_FRAGMENT_SHADER);
	m_shader_loader->makeShaderProgram(vertexShaderID,fragmentShaderID);
	printf("vertexShaderID is %d\n",vertexShaderID);
	printf("fragmentShaderID is %d\n",fragmentShaderID);
	printf("shaderProgramID is %d\n",m_shader_loader->GetProgramID());
}

float Lab3Model::GetRunningTime()
{
	return (float)((double) GetCurrentTimeMillis() - startTime) / 1000.0f;
}

void Lab3Model::staging()
{
	if(!timeFlag){
		deltaTime += 0.3f;
		if (deltaTime > 10)
		{
			timeFlag = true;
		}
	}
	if (timeFlag){
		deltaTime -= 0.3f;
		if (deltaTime < 0)
		{
			timeFlag = false;

		}
	}

	if (thirdCamera)
	{
		glm::mat4 stag1 = glm::translate(glm::mat4(1),glm::vec3(humanSkeleton->cModelPos.x,humanSkeleton->cModelPos.y+11+deltaTime,humanSkeleton->cModelPos.z+3));
		stag[0]->update(stag1, m_shader_loader->GetProgramID());
		stag[0]->draw();

		glm::mat4 stag2 = glm::translate(glm::mat4(1),glm::vec3(humanSkeleton->cModelPos.x,humanSkeleton->cModelPos.y+9+deltaTime,humanSkeleton->cModelPos.z+3));
		stag[1]->update(stag2, m_shader_loader->GetProgramID());
		stag[1]->draw();
	}

	if (thirdCamera_2)
	{
		glm::mat4 stag1 = glm::translate(glm::mat4(1),glm::vec3(humanSkeleton->hModelPos.x,humanSkeleton->hModelPos.y+12+deltaTime,humanSkeleton->hModelPos.z));
		stag[0]->update(stag1, m_shader_loader->GetProgramID());
		stag[0]->draw();

		glm::mat4 stag2 = glm::translate(glm::mat4(1),glm::vec3(humanSkeleton->hModelPos.x,humanSkeleton->hModelPos.y+10+deltaTime,humanSkeleton->hModelPos.z));
		stag[1]->update(stag2, m_shader_loader->GetProgramID());
		stag[1]->draw();
	}
}