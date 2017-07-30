#ifndef PENI3S_TECHNIQUE_H
#define PENI3S_TECHNIQUE_H

#include "gl_wrapper.h"
#include "mesh.h"
#include "math.h"

class Technique {
public:
    Technique();
    ~Technique();

    void draw(Mesh* mesh, mat4 projection, mat4 view_model);

private:
    GLuint program_;
    GLint uloc_projection_;
    GLint uloc_model_view_;
};

#endif //PENI3S_TECHNIQUE_H
