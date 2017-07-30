#include "gl_wrapper.h"
#include "mesh.h"
#include <unordered_map>
#include <android/log.h>

MeshPart::MeshPart(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
    int error;
    __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got mesh part (%u, %u)", vertices.size(), indices.size());
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on gen (%d)", error);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on buffer data (%d)", error);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on buffer attribs (%d)", error);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on buffer ebo (%d)", error);
    count_ = indices.size();
}

MeshPart::~MeshPart() { }

void MeshPart::emit() {
    int error;
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on before (%d)", error);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBindVertexArray(vao_);
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on buffer vao (%d)", error);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on buffer vbo (%d)", error);
    glDrawElements(GL_TRIANGLES, count_, GL_UNSIGNED_INT, nullptr);
    error = glGetError();
    if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error on buffer draw emit (%d)", error);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
    std::vector<Vertex> part_vertices;
    std::vector<GLuint> part_indices;
    std::unordered_map<GLuint, GLuint> part_id_map;
    const int batch_size = 500000;
    if (indices.size() < batch_size) {
        parts_.push_back(MeshPart(vertices, indices));
    } else {
        for (int i = 0; i < indices.size(); i += batch_size) {
            part_vertices.clear();
            part_indices.clear();
            part_id_map.clear();
            for (int j = i; j < i + batch_size and j < indices.size(); j++) {
                __android_log_print(ANDROID_LOG_INFO, "peni3s", "iter: %d", j);
                int orig_index = indices[j];
                auto mit = part_id_map.find(orig_index);
                if (mit != part_id_map.end()) {
                    part_indices.push_back(mit->second);
                } else {
                    int new_index = part_vertices.size();
                    part_indices.push_back(new_index);
                    part_id_map[orig_index] = new_index;
                    part_vertices.push_back(vertices[orig_index]);
                }
            }
            parts_.push_back(MeshPart(part_vertices, part_indices));
        }
    }
}

void Mesh::emit() {
    for (auto& part : parts_) {
        part.emit();
    }
}
