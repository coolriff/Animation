#pragma once
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/scene.h> // Assimp file
#include <assimp/mesh.h> // Assimp file
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include "TextureLoader.h"
#include "Shader.h"
#include <map>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class MeshLoader
{
public:
	static enum BUFFERS { VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER }; 

	MeshLoader(GLuint initialShaderID, const char* filename);
	~MeshLoader(void);

	void LoadMesh(const char* filename);

	void SetTexture(const char* filename);
	void SetColor(glm::vec3 color);
	void SetShader(GLuint shaderID) {this->shaderID = shaderID; SetAttributesAndUniforms();}
	GLuint GetShader(){return shaderID;}
	void IsTextureActive(bool useTexture) {this->useTexture = useTexture;}

	void SetPos(glm::vec3 position){this->position = position;}
	glm::vec3 GetPos(){return position;}

	void SetOrientation(glm::quat orientation) {this->orientation = orientation;}
	glm::quat GetOrientation() {return orientation;}

	void SetScale(glm::vec3 scale) {this->scale = scale;}
	glm::vec3 GetScale() {return scale;}

	//void SetShaderType(Shader::ShaderType shaderType) {this->shaderType = shaderType;}
	//Shader::ShaderType GetShaderType() {return shaderType;}

	std::vector<glm::vec3> GetVertices() { return vertices; }
	std::vector<glm::vec3> GetPoints() { return points; }

	void SetPossibleShaders( GLuint possibleShaders) { this->possibleShaders = possibleShaders;} 
	void SetAttributesAndUniforms();

	glm::mat4 GetTransformationMatrix();

	void UseProgram(){glUseProgram(shaderID); }

	void Rotate360(); // for rendering
	void VerticesToPoints(std::vector<glm::vec3> vert);
	void Update(glm::mat4 view, glm::mat4 proj, float deltaTime);
	void UpdateShader();
	//void InitMaterials(cons)
	void Render();
	void RenderPoly();

public:
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	glm::vec3 color;
	glm::vec3 previousColor;

	//Shader::ShaderType shaderType;

	GLuint possibleShaders;
	bool useTexture;

	//std::vector<Texture *> matTextures;
private:
	GLuint VAO, VBO[4];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> points;
	std::vector<glm::vec4> colors;

	GLuint vSize, cSize, numElements;

	TextureLoader *texture;
	GLuint gSampler, vColor;
	GLuint shaderID;

	std::string filename;
	GLuint modelLoc, viewLoc, projLoc;
};

