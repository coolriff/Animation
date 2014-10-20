#include "Lab1.h"


int main( int argc, char** argv )
{
	Lab1* lab1 = new Lab1();
	lab1->run();

	return 0;
}


/*
compile a vertex shader (get an ID)
compile a fragment shader (get an ID)
check for compilation errors
link those two shaders together (get an ID)
keep that ID
use that ID before you render triangles
can have separate shaders for each model

============================================================
sharder
============================================================
1-greate shader glCreateShader
2-bind the sharder to the source code glShaderSource
3-compile sharder glCompileShader 

4-create program ID glCreateProgram
5-attach shaders (fs,vs) glAttchShader
6-link program glLinkProgram
7- use program glUseProgram

============================================================
buffer
============================================================
create buffer -- bluint buffer -- glGenBuffers
bind the buffer -- glGenBuffers -- glBufferDate(size of the buffer)
load the geometry -- glBufferSubDate(geometry)
load the colors if any -- glBufferSubDate(color)
load the normals -- glBufferSubDate(color)

*/

