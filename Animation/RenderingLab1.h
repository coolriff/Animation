#pragma once
#include "Game.h"
#include "Mesh.h"

class RenderingLab1 : public Game
{
public:
	RenderingLab1(void);
	~RenderingLab1(void) {};

	void run() override;
	void init(char** argv);
	void initTweakBar();
	void initShaders() override;
	void initModels();

	Shader *shader1;
	TwBar *bar;
	Mesh *cube;

};

