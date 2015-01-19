#pragma once
#include <GL/glew.h>

namespace ContentLoader
{
	bool loadTexture(const char *filename, GLuint *outTexId);
}