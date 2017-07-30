#include "technique.h"
#include <android/log.h>
#include <cstring>
#include <sstream>


const static char* vertex_shader_source =
        "#version 300 es\n"
        "layout(location=0) in vec3 position;\n"
        "layout(location=1) in vec3 color;\n"
        "layout(location=2) in vec3 normal;\n"
        "uniform mat4 model_view;\n"
        "uniform mat4 projection;\n"
        "smooth out vec3 fs_color;\n"
        "smooth out vec3 fs_normal;\n"
        "void main() {\n"
        "  mat4 it_mv = transpose(inverse(model_view));\n"
        "  mat4 mvp = projection * model_view;\n"
        "  gl_Position = mvp * vec4(position, 1);\n"
        "  fs_normal = normalize(vec3(it_mv * vec4(normal, 0)));"
        "  fs_color = color;"
        "}\n";

const static char* frag_shader_source =
        "#version 300 es\n"
        "smooth in vec3 fs_color;\n"
        "smooth in vec3 fs_normal;\n"
        "out vec4 fs_out;\n"
        "void main() {\n"
        "  float diffuse = dot(vec3(0, 0, 1), fs_normal);\n"
        "  fs_out = vec4(diffuse * fs_color, 0);\n"
        "}\n";

static GLuint compile_shader(GLenum type, const GLchar* source, const char* shader_name = "shader") {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "peni3s", "Failed to create %s.", shader_name);
        return shader;
    }
    GLint length = strlen(source);
    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);

    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == 0) {
        int log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> buffer;
        const char* log = "No log available.";
        if (log_length != 0) {
            buffer.resize(log_length + 1);
            glGetShaderInfoLog(shader, log_length + 1, nullptr, buffer.data());
            *buffer.end() = '\0';
            log = buffer.data();
        }
        glDeleteShader(shader);
        __android_log_print(ANDROID_LOG_ERROR, "peni3s", "Failed to compile %s:\n%s", shader_name, log);
        return 0;
    }
    return shader;
}

static GLuint link_program(std::vector<GLuint> shaders, const char* program_name = "program") {
    GLuint program = glCreateProgram();
    if (program == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "peni3s", "Failed to create %s.", program_name);
        return 0;
    }
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);
    for (GLuint shader : shaders) {
        glDetachShader(program, shader);
    }
    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if (is_linked == 0) {
        int log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> buffer;
        const char* log = "No log available.";
        if (log_length != 0) {
            buffer.resize(log_length + 1);
            glGetProgramInfoLog(program, log_length + 1, nullptr, buffer.data());
            *buffer.end() = '\0';
            log = buffer.data();
        }
        glDeleteProgram(program);
        __android_log_print(ANDROID_LOG_ERROR, "peni3s", "Failed to link %s:\n%s", program_name, log);
        return 0;
    }
    return program;
}

Technique::Technique()
{
    GLuint vert = compile_shader(GL_VERTEX_SHADER, vertex_shader_source, "vertex shader");
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_shader_source, "fragment shader");
    program_ = link_program({vert, frag}, "technique program");
    glDeleteShader(vert);
    glDeleteShader(frag);
    uloc_projection_ = glGetUniformLocation(program_, "projection");
    uloc_model_view_ = glGetUniformLocation(program_, "model_view");
}

void Technique::draw(Mesh* mesh, mat4 projection, mat4 model_view) {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program_);
    glUniformMatrix4fv(uloc_projection_, 1, GL_FALSE, projection.data);
    glUniformMatrix4fv(uloc_model_view_, 1, GL_FALSE, model_view.data);
    mesh->emit();
}

Technique::~Technique() { }
