#include "Shader.h"


Shader::Shader(void)
{
}


Shader::~Shader(void)
{
}

bool Shader::readShaderFile(const std::string& fileName, std::string& outShader)
{
	std::ifstream file(fileName);

	if(!file.is_open())
	{
		std::cout << "Error Loading file: " << fileName << " - impossible to open file" << std::endl;
		return false;
	}

	if(file.fail())
	{
		std::cout << "Error Loading file: " << fileName << std::endl;
		return false;
	}
	else
	{
		std::cout << "Success loading file: : " << fileName << std::endl;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	outShader = stream.str();

	return true;
}

GLuint Shader::makeShader(const char* shaderText, GLenum shaderType)
{
	GLuint fsAndVsShadersID = glCreateShader(shaderType);
	glShaderSource(fsAndVsShadersID, 1, (const GLchar**)&shaderText, NULL);
	glCompileShader(fsAndVsShadersID);
	bool compiledCorrectly = checkCompiledShaderID(fsAndVsShadersID);
	if (compiledCorrectly)
	{
		return fsAndVsShadersID;
	}
	printf("SharderID not compiled");
	return -1;
}

void Shader::makeShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);

	GLint success = 0;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	if (success == 0) 
	{
		GLchar errorLog[1024];
		glGetProgramInfoLog(shaderProgramID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}
	else
	{
		std::cout << "Program Linked" << std::endl;
	}
}

bool Shader::checkCompiledShaderID(GLuint fsAndVsShadersID)
{
	GLint compiled = 0;
	glGetShaderiv(fsAndVsShadersID, GL_COMPILE_STATUS, &compiled);
	if (compiled)
	{
		return true;
	}
	else
	{
		GLint logLength;
		glGetShaderiv(fsAndVsShadersID, GL_COMPILE_STATUS, &logLength);
		char* msgBuffer = new char[logLength];
		glGetShaderInfoLog(fsAndVsShadersID, logLength, NULL, msgBuffer);
		printf("%s\n",msgBuffer);
		delete(msgBuffer);
		return false;
	}
}

GLuint Shader::GetProgramID()
{
	return shaderProgramID;
}


void Shader::findAllShaderID()
{
// 	vPositionID = glGetAttribLocation(shaderProgramID, "vPosition");
// 	vColorID = glGetAttribLocation(shaderProgramID, "vColor");
// 	vNormalID = glGetAttribLocation(shaderProgramID, "vNormal");

// 	lightPosition = glGetUniformLocation(shaderProgramID, "Light.Position");
// 	lightIntensity = glGetUniformLocation(shaderProgramID, "Light.Intensity");
// 	materialAmbient = glGetUniformLocation(shaderProgramID, "Material.Ka");
// 	materialDiffuse = glGetUniformLocation(shaderProgramID, "Material.Kd");
// 	materiaSpecular = glGetUniformLocation(shaderProgramID, "Material.Ks");
// 	materiaShininess = glGetUniformLocation(shaderProgramID, "Material.Shininess");

	vLightDir = glGetUniformLocation(shaderProgramID, "vLightDir");
	std::cout << vLightDir << std::endl;
	ambientColor = glGetUniformLocation(shaderProgramID, "ambientColor");
	std::cout << ambientColor << std::endl;
	specularColor = glGetUniformLocation(shaderProgramID, "specularColor");
	std::cout << specularColor << std::endl;
	diffuseColor = glGetUniformLocation(shaderProgramID, "diffuseColor");
	std::cout << diffuseColor << std::endl;

	ambientIntensity = glGetUniformLocation(shaderProgramID, "ambientIntensity");
	std::cout << ambientIntensity << std::endl;
	specularIntensity = glGetUniformLocation(shaderProgramID, "specularIntensity");
	std::cout << specularIntensity << std::endl;
	diffuseIntensity = glGetUniformLocation(shaderProgramID, "diffuseIntensity");
	std::cout << diffuseIntensity << std::endl;
	specularShininess = glGetUniformLocation(shaderProgramID, "specularShininess");
	std::cout << specularShininess << std::endl;
}

void Shader::SetRoughness(float roughness)
{
/*	glUniform1f(roughnessID, roughness);*/
}


void Shader::SetEyeVector(const glm::vec3 &eye)
{
/*	glUniform3f(eyeID, eye.x, eye.y, eye.z);*/
}

void Shader::SetAmbientLight(glm::vec3 color, float intensity)
{
// 	glUniform3f(ambientColorID, color.r, color.g, color.b);
// 	glUniform1f(ambientIntensityID, intensity);
}

void Shader::SetDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
{
// 	glUniform3f(diffuseColorID, color.r, color.g, color.b);
// 	glUniform1f(diffuseIntensityID, intensity);
// 	glUniform3f(diffuseDirectionID, direction.x, direction.y, direction.z);
}

void Shader::SetSpecularComponent(glm::vec3 color, float intensity, float shininess)
{
// 	glUniform3f(specularColorID, color.r, color.g, color.b);
// 	glUniform1f(specularIntensityID, intensity);
// 	glUniform1f(specularShininessID, shininess);
}


void Shader::SetAll(glm::vec3 vLightDirGLM,glm::vec3 ambientColorGLM,glm::vec3 specularColorGLM,glm::vec3 diffuseColorGLM,
					float ambientIntensityGLM,float specularIntensityGLM,float diffuseIntensityGLM,float specularShininessGLM)
{
	glUniform3f(vLightDir, vLightDirGLM.x,vLightDirGLM.y,vLightDirGLM.z);
	glUniform3f(ambientColor,ambientColorGLM.x,ambientColorGLM.y,ambientColorGLM.z);
	glUniform3f(specularColor,specularColorGLM.x,specularColorGLM.y,specularColorGLM.z);
	glUniform3f(diffuseColor,diffuseColorGLM.x,diffuseColorGLM.y,diffuseColorGLM.z);
	glUniform1f(ambientIntensity,ambientIntensityGLM);
	glUniform1f(specularIntensity,specularIntensityGLM);
	glUniform1f(diffuseIntensity,diffuseIntensityGLM);
	glUniform1f(specularShininess,specularShininessGLM);
}
