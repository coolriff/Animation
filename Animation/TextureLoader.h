#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <gl/glew.h>
#include <SOIL.h>

#include <iostream>

class TextureLoader
{
public:
	TextureLoader(GLenum textureTarget, const std::string &filename);
	TextureLoader(const char* directory);

	bool Load();
	void Bind(GLenum textureUnit);
	void UnBind();
	bool LoadCubeMap();
	bool LoadTGA();
	std::string filename;
	const char *directory;
	GLenum textureTarget;
	GLuint textureObj;
};

#endif // !TEXTURELOADER_H
