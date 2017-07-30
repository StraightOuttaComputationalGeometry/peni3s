#ifndef PENI3S_MESH_H
#define PENI3S_MESH_H

#include "gl_wrapper.h"
#include "math.h"
#include <vector>

struct Vertex {
    vec3 position;
    vec3 color;
    vec3 normal;

    Vertex(vec3 position, vec3 color, vec3 normal)
      : position (position), color (color), normal (normal)
    { }
};

struct MeshPart {
    GLsizei count_;
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

    MeshPart(std::vector<Vertex> vertices,
             std::vector<GLuint> indices);
    ~MeshPart();

    void emit();
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<GLuint> indices);

    void emit();

private:
    std::vector<MeshPart> parts_;
};


#endif //PENI3S_MESH_H
