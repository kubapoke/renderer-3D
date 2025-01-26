#ifndef RENDERER3D_RENDERABLE_H
#define RENDERER3D_RENDERABLE_H

#include "Shader.h"

class Renderable{
public:
    virtual void Draw(Shader &shader) = 0;
};

#endif //RENDERER3D_RENDERABLE_H
