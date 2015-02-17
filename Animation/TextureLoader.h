#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <gl/glew.h>
#include <SOIL.h>

#include <iostream>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	bool Load(GLenum textureTarget, const std::string &filename);
	bool LoadSkyBox(GLenum textureTarget, const std::string &filename);
	void Bind(GLenum textureUnit);
	void UnBind();

	std::string filename;
	GLenum textureTarget;
	GLuint textureObj;
};

#endif // !TEXTURELOADER_H
