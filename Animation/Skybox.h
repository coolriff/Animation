#ifndef SKYBOX_H
#define SKYBOX_H

#include "glm/glm.hpp"
#include <GL/glew.h>

class SkyBox
{
private:
    unsigned int vaoHandle;

public:
    SkyBox();

    void render() const;
};


#endif // SKYBOX_H
