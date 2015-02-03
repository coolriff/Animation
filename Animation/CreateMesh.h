#ifndef CREATEMEASH_H
#define CREATEMEASH_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class CreateMesh
{
public:

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec4> redColors;
	std::vector<glm::vec4> greenColors;
	std::vector<glm::vec4> blueColors;
	std::vector<glm::vec3> normals;

	void createCubeMesh();
	void createBoundingSphereMesh(float radius, int resolution);
	void createSphereMesh(int numSegments);
	void setColors(glm::vec4 c);
	std::vector<glm::vec4> getColors(void) {return colors;};
};
#endif

