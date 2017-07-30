#ifndef PENI3S_MATH_H
#define PENI3S_MATH_H

#include "gl_wrapper.h"
#include <cmath>
#include <sstream>

struct vec3 {
    vec3(GLfloat x, GLfloat y, GLfloat z) : data{x, y, z} {}

    GLfloat data[3];

    vec3 scale(GLfloat factor) {
        return vec3(data[0] * factor,
                    data[1] * factor,
                    data[2] * factor);
    }

    vec3 add(vec3 other) {
        return vec3(data[0] + other.data[0],
                    data[1] + other.data[1],
                    data[2] + other.data[2]);
    }

    vec3 sub(vec3 other) {
        return vec3(data[0] - other.data[0],
                    data[1] - other.data[1],
                    data[2] - other.data[2]);
    }

    GLfloat dot(vec3 other) {
        return data[0] * other.data[0] + data[1] * other.data[1] + data[2] * other.data[2];
    }

    vec3 cross(vec3 other) {
        return vec3(data[1] * other.data[2] - data[2] * other.data[1],
                    data[2] * other.data[0] - data[0] * other.data[2],
                    data[0] * other.data[1] - data[1] * other.data[0]);
    }

    vec3 normalize() {
        GLfloat total = data[0] + data[1] + data[2];
        if (total == 0.) {
            return vec3(0.f, 0.f, 0.f);
        }
        return scale(1.f / total);
    }

    std::string to_string() const {
        std::stringstream builder;
        builder << "{" << data[0] << ", " << data[1] << ", " << data[2] << "}";
        return builder.str();
    }
};

struct mat4 {
    GLfloat data [16];

    mat4(   GLfloat m00, GLfloat m10, GLfloat m20, GLfloat m30,
            GLfloat m01, GLfloat m11, GLfloat m21, GLfloat m31,
            GLfloat m02, GLfloat m12, GLfloat m22, GLfloat m32,
            GLfloat m03, GLfloat m13, GLfloat m23, GLfloat m33) :
    data { m00, m10, m20, m30,
           m01, m11, m21, m31,
           m02, m12, m22, m32,
           m03, m13, m23, m33
    }
    { }

    static mat4 look_at(vec3 eye, vec3 at, vec3 up) {
        vec3 zaxis = eye.sub(at).normalize();
        vec3 xaxis = up.cross(zaxis).normalize();
        vec3 yaxis = zaxis.cross(xaxis).normalize();
        return mat4 (
                xaxis.data[0], yaxis.data[0], zaxis.data[0], 0,
                xaxis.data[1], yaxis.data[1], zaxis.data[1], 0,
                xaxis.data[2], yaxis.data[2], zaxis.data[2], 0,
                -eye.dot(xaxis), -eye.dot(yaxis), -eye.dot(zaxis), 1
        );
    }

    static mat4 perspective(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
        GLfloat f = 1.f / tan(fovy / 2.f);
        return mat4 (
                f * aspect, 0.f, 0.f, 0.f,
                0.f, f, 0.f, 0.f,
                0.f, 0.f, (far + near) / (near - far), -1.f,
                0.f, 0.f, 2 * far * near / (near - far), 0.f
        );
    }

    static mat4 translate(GLfloat x, GLfloat y, GLfloat z) {
       return mat4 (
               1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               x, y, z, 1
       );
    }

    static mat4 scale(GLfloat amount) {
        return mat4 (
                amount, 0, 0, 0,
                0, amount, 0, 0,
                0, 0, amount, 0,
                0, 0, 0, 1
        );
    }

    static mat4 rotate_x(GLfloat angle) {
        return mat4 (
                1, 0, 0, 0,
                0, cos(angle), -sin(angle), 0,
                0, sin(angle), cos(angle), 0,
                0, 0, 0, 1
        );
    }

    static mat4 rotate_y(GLfloat angle) {
        return mat4 (
                cos(angle), 0, sin(angle), 0,
                0, 1, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1
        );
    }

    static mat4 rotate_z(GLfloat angle) {
        return mat4 (
                cos(angle),-sin(angle), 0, 0,
                sin(angle), cos(angle), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        );
    }

    mat4 mult(mat4 other) {
        float m00 = (data[0 * 4 + 0] * other.data[0 * 4 + 0]) +
                    (data[1 * 4 + 0] * other.data[0 * 4 + 1]) +
                    (data[2 * 4 + 0] * other.data[0 * 4 + 2]) +
                    (data[3 * 4 + 0] * other.data[0 * 4 + 3]);
        float m01 = (data[0 * 4 + 0] * other.data[1 * 4 + 0]) +
                    (data[1 * 4 + 0] * other.data[1 * 4 + 1]) +
                    (data[2 * 4 + 0] * other.data[1 * 4 + 2]) +
                    (data[3 * 4 + 0] * other.data[1 * 4 + 3]);
        float m02 = (data[0 * 4 + 0] * other.data[2 * 4 + 0]) +
                    (data[1 * 4 + 0] * other.data[2 * 4 + 1]) +
                    (data[2 * 4 + 0] * other.data[2 * 4 + 2]) +
                    (data[3 * 4 + 0] * other.data[2 * 4 + 3]);
        float m03 = (data[0 * 4 + 0] * other.data[3 * 4 + 0]) +
                    (data[1 * 4 + 0] * other.data[3 * 4 + 1]) +
                    (data[2 * 4 + 0] * other.data[3 * 4 + 2]) +
                    (data[3 * 4 + 0] * other.data[3 * 4 + 3]);
        float m10 = (data[0 * 4 + 1] * other.data[0 * 4 + 0]) +
                    (data[1 * 4 + 1] * other.data[0 * 4 + 1]) +
                    (data[2 * 4 + 1] * other.data[0 * 4 + 2]) +
                    (data[3 * 4 + 1] * other.data[0 * 4 + 3]);
        float m11 = (data[0 * 4 + 1] * other.data[1 * 4 + 0]) +
                    (data[1 * 4 + 1] * other.data[1 * 4 + 1]) +
                    (data[2 * 4 + 1] * other.data[1 * 4 + 2]) +
                    (data[3 * 4 + 1] * other.data[1 * 4 + 3]);
        float m12 = (data[0 * 4 + 1] * other.data[2 * 4 + 0]) +
                    (data[1 * 4 + 1] * other.data[2 * 4 + 1]) +
                    (data[2 * 4 + 1] * other.data[2 * 4 + 2]) +
                    (data[3 * 4 + 1] * other.data[2 * 4 + 3]);
        float m13 = (data[0 * 4 + 1] * other.data[3 * 4 + 0]) +
                    (data[1 * 4 + 1] * other.data[3 * 4 + 1]) +
                    (data[2 * 4 + 1] * other.data[3 * 4 + 2]) +
                    (data[3 * 4 + 1] * other.data[3 * 4 + 3]);
        float m20 = (data[0 * 4 + 2] * other.data[0 * 4 + 0]) +
                    (data[1 * 4 + 2] * other.data[0 * 4 + 1]) +
                    (data[2 * 4 + 2] * other.data[0 * 4 + 2]) +
                    (data[3 * 4 + 2] * other.data[0 * 4 + 3]);
        float m21 = (data[0 * 4 + 2] * other.data[1 * 4 + 0]) +
                    (data[1 * 4 + 2] * other.data[1 * 4 + 1]) +
                    (data[2 * 4 + 2] * other.data[1 * 4 + 2]) +
                    (data[3 * 4 + 2] * other.data[1 * 4 + 3]);
        float m22 = (data[0 * 4 + 2] * other.data[2 * 4 + 0]) +
                    (data[1 * 4 + 2] * other.data[2 * 4 + 1]) +
                    (data[2 * 4 + 2] * other.data[2 * 4 + 2]) +
                    (data[3 * 4 + 2] * other.data[2 * 4 + 3]);
        float m23 = (data[0 * 4 + 2] * other.data[3 * 4 + 0]) +
                    (data[1 * 4 + 2] * other.data[3 * 4 + 1]) +
                    (data[2 * 4 + 2] * other.data[3 * 4 + 2]) +
                    (data[3 * 4 + 2] * other.data[3 * 4 + 3]);
        float m30 = (data[0 * 4 + 3] * other.data[0 * 4 + 0]) +
                    (data[1 * 4 + 3] * other.data[0 * 4 + 1]) +
                    (data[2 * 4 + 3] * other.data[0 * 4 + 2]) +
                    (data[3 * 4 + 3] * other.data[0 * 4 + 3]);
        float m31 = (data[0 * 4 + 3] * other.data[1 * 4 + 0]) +
                    (data[1 * 4 + 3] * other.data[1 * 4 + 1]) +
                    (data[2 * 4 + 3] * other.data[1 * 4 + 2]) +
                    (data[3 * 4 + 3] * other.data[1 * 4 + 3]);
        float m32 = (data[0 * 4 + 3] * other.data[2 * 4 + 0]) +
                    (data[1 * 4 + 3] * other.data[2 * 4 + 1]) +
                    (data[2 * 4 + 3] * other.data[2 * 4 + 2]) +
                    (data[3 * 4 + 3] * other.data[2 * 4 + 3]);
        float m33 = (data[0 * 4 + 3] * other.data[3 * 4 + 0]) +
                    (data[1 * 4 + 3] * other.data[3 * 4 + 1]) +
                    (data[2 * 4 + 3] * other.data[3 * 4 + 2]) +
                    (data[3 * 4 + 3] * other.data[3 * 4 + 3]);
        return mat4 (m00, m10, m20, m30,
                     m01, m11, m21, m31,
                     m02, m12, m22, m32,
                     m03, m13, m23, m33);
    }

    std::string to_string() const {
        std::stringstream builder;
        builder << "{\t" << data[0] << "\t" << data[4] << "\t" << data[8] << "\t" << data[12]  << "\n"
                << "\t"  << data[1] << "\t" << data[5] << "\t" << data[9] << "\t" << data[13]  << "\n"
                << "\t"  << data[2] << "\t" << data[6] << "\t" << data[10] << "\t" << data[14] << "\n"
                << "\t"  << data[3] << "\t" << data[7] << "\t" << data[11] << "\t" << data[15] << "}";
        return builder.str();
    }
};


#endif //PENI3S_MATH_H
