#include "TextureLoader.h"

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{

}

bool TextureLoader::Load(GLenum textureTarget, const std::string &filename)
{
	this->textureTarget = textureTarget;
	this->filename = filename;

	int width,height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

	glGenTextures(1, &textureObj);
	glBindTexture(textureTarget, textureObj);
	glTexImage2D(textureTarget, 0 , GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,image);

	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
	glBindTexture(textureTarget, 0);

	return true;
}

bool TextureLoader::LoadSkyBox(GLenum textureTarget, const std::string &filename)
{
	this->textureTarget = textureTarget;
	this->filename = filename;

	int width,height;

	glGenTextures(1, &textureObj);
	glBindTexture(textureTarget, textureObj);

	std::string right = std::string(filename.c_str()) + "right.jpg";
	std::string left = std::string(filename.c_str()) + "left.jpg";
	std::string up = std::string(filename.c_str()) + "top.jpg";
	std::string down = std::string(filename.c_str()) + "bottom.jpg";
	std::string back = std::string(filename.c_str()) + "back.jpg";
	std::string front = std::string(filename.c_str()) + "front.jpg";

	textureObj = SOIL_load_OGL_cubemap(right.c_str(),left.c_str(), up.c_str(), down.c_str(), back.c_str(), front.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS);

	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);   
	glBindTexture(textureTarget, 0);

	return true;
}

void TextureLoader::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObj);
}

void TextureLoader::UnBind()
{
	glBindTexture(textureTarget, 0);
}

