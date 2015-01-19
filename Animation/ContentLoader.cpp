
#include <assert.h>
#include "ContentLoader.h"
#include <GL/glew.h>
#include "SOIL.h"
#include <iostream>

#pragma comment(lib, "soil")

namespace ContentLoader
{
	bool loadTexture(const char *filename, GLuint *outTexId)
	{
		assert(outTexId);
		*outTexId = SOIL_load_OGL_texture
			(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			/*SOIL_FLAG_GL_MIPMAPS | */SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
			);

		if (*outTexId == 0)
		{
			std::cout <<("Cannot load texture: %s", filename) << std::endl;
		}

		return *outTexId != 0;
	}
}