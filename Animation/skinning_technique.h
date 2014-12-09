#ifndef SKINNING_TECHNIQUE_H
#define	SKINNING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"
#include <glm/glm.hpp>
#include <gl/glew.h>

class SkinningTechnique
{
public:

    static const GLuint MAX_BONES = 100;

    SkinningTechnique();

    virtual bool Init(GLuint shaderProgramID);

    //void SetWVP(const glm::mat4& WVP);
    //void SetWorldMatrix(const glm::mat4& WVP);
    void SetColorTextureUnit(GLuint TextureUnit);
    //void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
    void SetBoneTransform(GLuint Index, const Matrix4f& Transform);

private:
    
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_colorTextureLocation;
    GLuint m_eyeWorldPosLocation;
    
    GLuint m_boneLocation[MAX_BONES];

	GLuint shaderProgramID;
};


#endif	/* SKINNING_TECHNIQUE_H */
