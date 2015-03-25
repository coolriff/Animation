#ifndef CREATEMEASH_H
#define CREATEMEASH_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "TextureLoader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class CreateMesh
{
public:
	CreateMesh(void);
	~CreateMesh(void);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> indices;
	std::vector<glm::vec4> colors;

	std::vector<glm::vec4> redColors;
	std::vector<glm::vec4> greenColors;
	std::vector<glm::vec4> blueColors;

	GLuint numElements;
	GLuint vSize, cSize;
	GLuint VAO, v_VBO, t_VBO, n_VBO, i_VBO, tb_VBO;
	GLuint gSampler;
	GLuint nSampler;

	bool isTextured;
	bool isNormalMap;

	void createCubeMesh();
	void createBoundingSphereMesh(float radius, int resolution);
	void createSphereMesh(int numSegments);
	void setColors(glm::vec4 c);
	void setTexture(const char* filename, GLuint shaderID);
	std::vector<glm::vec4> getColors(void) {return colors;};
	void loadMesh(const char* filename);
	void render();
	void renderSkyBox();
	void setNormalTexture(const char* textureName, GLuint shaderID);
	void setCubeMapTexture(const char* directory, GLuint shaderID);
	void renderNormalMap(GLuint shaderID);
	void loadMeshNormal(const char* filename);
	void setTgaTexture(const char* textureName, GLuint shaderID);
	TextureLoader *texture;
	TextureLoader *normalTexture;
	TextureLoader *cubeTexture;
};
#endif

