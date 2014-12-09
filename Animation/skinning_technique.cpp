#include <limits.h>
#include <string>
#include <assert.h>
#include <iostream>
#include "skinning_technique.h"
#include "ogldev_util.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

SkinningTechnique::SkinningTechnique()
{   
}

bool SkinningTechnique::Init(GLuint shaderProgramID)
{ 
	this->shaderProgramID = shaderProgramID;

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) 
	{
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
        m_boneLocation[i] = glGetUniformLocation(shaderProgramID,Name);
    }

    return true;
}

void SkinningTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
	m_colorTextureLocation = glGetUniformLocation(shaderProgramID,"gColorMap");
    glUniform1i(m_colorTextureLocation, TextureUnit);
}

void SkinningTechnique::SetBoneTransform(GLuint Index, const Matrix4f& Transform)
{
	assert(Index < MAX_BONES);

    glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE,(const GLfloat*)Transform);       
}