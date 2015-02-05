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
	vPositionID = glGetAttribLocation(shaderProgramID, "vPosition");
	vColorID = glGetAttribLocation(shaderProgramID, "vColor");
	vNormalID = glGetAttribLocation(shaderProgramID, "vNormal");

	eyeID = glGetUniformLocation(shaderProgramID, "vEye");

	ambientColorID = glGetUniformLocation(shaderProgramID, "ambientColor");
	ambientIntensityID = glGetUniformLocation(shaderProgramID, "ambientIntensity");

	diffuseColorID = glGetUniformLocation(shaderProgramID, "diffuseColor");
	diffuseIntensityID = glGetUniformLocation(shaderProgramID, "diffuseIntensity");
	diffuseDirectionID = glGetUniformLocation(shaderProgramID, "diffuseDirection");

	specularColorID = glGetUniformLocation(shaderProgramID, "specularColor");
	specularIntensityID = glGetUniformLocation(shaderProgramID, "specularIntensity");
	specularShininessID = glGetUniformLocation(shaderProgramID, "specularShininess");

	roughnessID = glGetUniformLocation(shaderProgramID, "roughness");
}

void Shader::SetRoughness(float roughness)
{
	glUniform1f(roughnessID, roughness);
}


void Shader::SetEyeVector(const glm::vec3 &eye)
{
	glUniform3f(eyeID, eye.x, eye.y, eye.z);
}

void Shader::SetAmbientLight(glm::vec3 color, float intensity)
{
	glUniform3f(ambientColorID, color.r, color.g, color.b);
	glUniform1f(ambientIntensityID, intensity);
}

void Shader::SetDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
{
	glUniform3f(diffuseColorID, color.r, color.g, color.b);
	glUniform1f(diffuseIntensityID, intensity);
	glUniform3f(diffuseDirectionID, direction.x, direction.y, direction.z);
}

void Shader::SetSpecularComponent(glm::vec3 color, float intensity, float shininess)
{
	glUniform3f(specularColorID, color.r, color.g, color.b);
	glUniform1f(specularIntensityID, intensity);
	glUniform1f(specularShininessID, shininess);
}
