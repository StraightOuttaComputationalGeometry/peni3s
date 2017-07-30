#include "gl_wrapper.h"
#include "graphics.h"
#include "tiny_obj_loader.h"
#include "mesh.h"
#include "technique.h"
#include <android/log.h>
#include <sstream>
#include <memory>

struct IndexComparator {
    bool operator()(const tinyobj::index_t &lhs, const tinyobj::index_t &rhs) {
        if (lhs.vertex_index != rhs.vertex_index) return lhs.vertex_index < rhs.vertex_index;
        return lhs.normal_index < rhs.normal_index;
    }
};

namespace graphics {

    float aspect = 1.;
    mat4 base_transform (1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    mat4 add_transform (
                         1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    float offset_x = 0.f;
    float offset_y = 0.f;
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Technique> technique;

    bool initialize(JNIEnv* env) {
        technique = std::unique_ptr<Technique>(new Technique());
        glClearColor(1.f, 0.f, 0.f, 0.f);
        return true;
    }

    void on_surface_changed(int width, int height) {
        aspect = height / static_cast<float>(width);
    }

    void on_draw_frame() {
        int error;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mat4 projection = mat4::perspective(1.f, aspect, 0.1f, 100.f);
        mat4 model_view = mat4::look_at(vec3(0.f, 0.f, 5.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f)).mult(
            mat4::translate(offset_x, offset_y, 0).mult(add_transform.mult(base_transform))
        );
        error = glGetError();
        if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error before (%d)", error);
        if (mesh and technique) {
            technique->draw(mesh.get(), projection, model_view);
        }
        error = glGetError();
        if (error) __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got GL error after (%d)", error);
    }

    void load_model_from_string(std::string source) {
        std::stringstream stream(source);
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string obj_error;
        if (not tinyobj::LoadObj(&attrib, &shapes, &materials, &obj_error, &stream)) {
            __android_log_print(ANDROID_LOG_ERROR, "peni3s", "Failed to load obj.");
            return;
        }
        auto query_vertex = [&](int i) {
            return vec3(attrib.vertices[3 * i + 0],
                        attrib.vertices[3 * i + 1],
                        attrib.vertices[3 * i + 2]);
        };
        auto query_vertex_color = [&](int i) {
            if (i >= attrib.colors.size()) {
                return vec3(1.f, 1.f, 1.f);
            }
            return vec3(attrib.colors[3 * i + 0],
                        attrib.colors[3 * i + 1],
                        attrib.colors[3 * i + 2]);
        };
        auto query_vertex_normal = [&](int i) {
            if (i >= attrib.normals.size()) {
                return vec3(0.f, 0.f, 1.f);
            }
            return vec3(attrib.normals[3 * i + 0],
                        attrib.normals[3 * i + 1],
                        attrib.normals[3 * i + 2]);
        };
        __android_log_print(ANDROID_LOG_INFO, "peni3s", "Loaded model %u, %u, %u",
                            attrib.vertices.size(), attrib.colors.size(), attrib.normals.size());
        if (obj_error.size() > 0) {
        __android_log_print(ANDROID_LOG_INFO, "peni3s", "Got error \"%s\"", obj_error.c_str());
        }
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::map<tinyobj::index_t, size_t, IndexComparator> ids;
        auto add_vertex = [&](tinyobj::index_t index) {
            auto it = ids.find(index);
            if (it != ids.end()) {
                indices.push_back(it->second);
            } else {
                indices.push_back(vertices.size());
                vertices.push_back(Vertex (
                    query_vertex(index.vertex_index),
                    query_vertex_color(index.vertex_index),
                    query_vertex_normal(index.normal_index)
                ));
            }
        };
        for (const auto& shape : shapes) {
            __android_log_print(ANDROID_LOG_INFO, "peni3s", "Found shape %u", shape.mesh.indices.size());
            for (auto index : shape.mesh.indices) {
                add_vertex(index);
            }
        }
        vec3 max_pos = vertices[0].position;
        vec3 min_pos = vertices[0].position;
        vec3 acc_pos = vertices[0].position;
        for (const auto& vertex : vertices) {
            acc_pos = acc_pos.add(vertex.position);
            max_pos = vec3(
                (max_pos.data[0] > vertex.position.data[0]) ? max_pos.data[0] : vertex.position.data[0],
                (max_pos.data[0] > vertex.position.data[0]) ? max_pos.data[0] : vertex.position.data[0],
                (max_pos.data[0] > vertex.position.data[0]) ? max_pos.data[0] : vertex.position.data[0]
            );
            min_pos = vec3(
                (min_pos.data[0] < vertex.position.data[0]) ? min_pos.data[0] : vertex.position.data[0],
                (min_pos.data[0] < vertex.position.data[0]) ? min_pos.data[0] : vertex.position.data[0],
                (min_pos.data[0] < vertex.position.data[0]) ? min_pos.data[0] : vertex.position.data[0]
            );
        }
        vec3 avg_pos = acc_pos.scale(1.f / vertices.size());
        vec3 dim = max_pos.sub(min_pos);
        float max_dim = dim.data[0];
        if (max_dim < dim.data[1]) max_dim = dim.data[1];
        if (max_dim < dim.data[2]) max_dim = dim.data[2];
        base_transform = mat4::scale(1 / max_dim).mult(
                mat4::translate(-avg_pos.data[0], -avg_pos.data[1], -avg_pos.data[2])
        );
        mesh = std::unique_ptr<Mesh>(new Mesh(vertices, indices));
    }

    void on_pan(float x, float y) {
        offset_x += x;
        offset_y += y;
    }

    void on_rotate(float x, float y) {
        add_transform = mat4::rotate_x(y).mult(mat4::rotate_y(-x).mult(add_transform));
    }

    void on_scale(float amount) {
        add_transform = mat4::scale(amount).mult(add_transform);
    }
}