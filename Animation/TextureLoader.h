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

	bool Load();
	void Bind(GLenum textureUnit);
	void UnBind();
	std::string filename;
	GLenum textureTarget;
	GLuint textureObj;
};

#endif // !TEXTURELOADER_H
