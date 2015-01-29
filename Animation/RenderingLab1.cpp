#include "RenderingLab1.h"

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


RenderingLab1::RenderingLab1(void)
{
	shader1 = new Shader();
	window = new Window(800,600,"Lab 1: Rendering");
	camera = new Camera(window->GetWindow());
}

void RenderingLab1::init(char** argv)
{
	initShaders();
	initModels();
	initTweakBar();
}

void RenderingLab1::initShaders()
{
	shader1->CreateProgram();

	shader1->LoadFile("../Resources/Shaders/objNoTexture.vs",vs);
	shader1->AddShader(shader1->GetProgramID(), vs.c_str(), GL_VERTEX_SHADER);

	shader1->LoadFile("../Resources/Shaders/objNoTexture.ps",ps);
	shader1->AddShader(shader1->GetProgramID(), ps.c_str(), GL_FRAGMENT_SHADER);

	shader1->LinkProgram();
	shader1->ValidateProgram();
	shader1->UseProgram();
}

void RenderingLab1::initModels()
{
	cube = new Mesh("../Resources/Models/Cube/cube.obj");
}

void RenderingLab1::run()
{
	while(!window->IsCloseRequested())
	{
		// calculating delta time

		window->Update();

		glUseProgram(shader1->GetProgramID());


		GLuint modelLoc = glGetUniformLocation(shader1->GetProgramID(), "model");
		GLuint viewLoc = glGetUniformLocation(shader1->GetProgramID(), "view");
		GLuint projLoc = glGetUniformLocation(shader1->GetProgramID(), "projection");

		camera->computeMatricesFromInputs();
		camera->handleMVP(modelLoc,viewLoc,projLoc);

		camera->getProjectionMatrix();
		camera->getViewMatrix();

		cube->render();

		TwDraw();

		window->SwapBuffers();
	}
}

void RenderingLab1::initTweakBar()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window->GetWidth(), window->GetHeight());

	bar = TwNewBar("Attributes: ");

	TwAddVarRW(bar, "Camera", TW_TYPE_DIR3F, &camera->position, " label='Camera Pos: '");
	TwAddVarRW(bar, "Camera Rot", TW_TYPE_DIR3F, &camera->direction, " label='Camera Rot: '");

	glfwSetMouseButtonCallback(window->GetWindow(),MouseButtonCB);
	glfwSetCursorPosCallback(window->GetWindow(),MousePosCB);
	glfwSetScrollCallback(window->GetWindow(),MouseScrollCB);
	glfwSetKeyCallback(window->GetWindow(),KeyFunCB);
}
