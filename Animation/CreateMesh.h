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

class CreateMesh
{
public:

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> indices;
	std::vector<glm::vec4> colors;

	std::vector<glm::vec4> redColors;
	std::vector<glm::vec4> greenColors;
	std::vector<glm::vec4> blueColors;

	GLuint numElements;

	void createCubeMesh();
	void createBoundingSphereMesh(float radius, int resolution);
	void createSphereMesh(int numSegments);
	void setColors(glm::vec4 c);
	std::vector<glm::vec4> getColors(void) {return colors;};
	void LoadMesh(const char* filename);
};
#endif

