/*
    src/glutil.cpp -- Convenience classes for accessing OpenGL >= 3.x

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "t_glutil.h"

#if defined(WIN32)
#  if !defined(__clang__)
#    include <malloc.h"
#  endif
#endif

#include <iostream>
#include <fstream>
#include <Eigen/Geometry>

namespace _ {

static GLuint createShader_helper(GLint type, const std::string &name,
                                  const std::string &defines,
                                  std::string shader_string) {
    if (shader_string.empty())
        return (GLuint) 0;

    if (!defines.empty()) {
        if (shader_string.length() > 8 && shader_string.substr(0, 8) == "#version") {
            std::istringstream iss(shader_string);
            std::ostringstream oss;
            std::string line;
            std::getline(iss, line);
            oss << line << std::endl;
            oss << defines;
            while (std::getline(iss, line))
                oss << line << std::endl;
            shader_string = oss.str();
        } else {
            shader_string = defines + shader_string;
        }
    }

    GLuint id = glCreateShader(type);
    const char *shader_string_const = shader_string.c_str();
    glShaderSource(id, 1, &shader_string_const, nullptr);
    glCompileShader(id);

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char buffer[512];
        std::cerr << "Error while compiling ";
        if (type == GL_VERTEX_SHADER)
            std::cerr << "vertex shader";
        else if (type == GL_FRAGMENT_SHADER)
            std::cerr << "fragment shader";
        else if (type == GL_GEOMETRY_SHADER)
            std::cerr << "geometry shader";
        std::cerr << " \"" << name << "\":" << std::endl;
        std::cerr << shader_string << std::endl << std::endl;
        glGetShaderInfoLog(id, 512, nullptr, buffer);
        std::cerr << "Error: " << std::endl << buffer << std::endl;
        throw std::runtime_error("Shader compilation failed!");
    }

    return id;
}

//  ----------------------------------------------------

Eigen::Vector3f project(const Eigen::Vector3f &obj,
                        const Eigen::Matrix4f &model,
                        const Eigen::Matrix4f &proj,
                        const Vector2i &viewportSize) {
    Eigen::Vector4f tmp;
    tmp << obj, 1;

    tmp = model * tmp;

    tmp = proj * tmp;

    tmp = tmp.array() / tmp(3);
    tmp = tmp.array() * 0.5f + 0.5f;
    tmp(0) = tmp(0) * viewportSize.x();
    tmp(1) = tmp(1) * viewportSize.y();

    return tmp.head(3);
}

Eigen::Vector3f unproject(const Eigen::Vector3f &win,
                          const Eigen::Matrix4f &model,
                          const Eigen::Matrix4f &proj,
                          const Vector2i &viewportSize) {
    Eigen::Matrix4f Inverse = (proj * model).inverse();

    Eigen::Vector4f tmp;
    tmp << win, 1;
    tmp(0) = tmp(0) / viewportSize.x();
    tmp(1) = tmp(1) / viewportSize.y();
    tmp = tmp.array() * 2.0f - 1.0f;

    Eigen::Vector4f obj = Inverse * tmp;
    obj /= obj(3);

    return obj.head(3);
}

Eigen::Matrix4f lookAt(const Eigen::Vector3f &origin,
                       const Eigen::Vector3f &target,
                       const Eigen::Vector3f &up) {
    Eigen::Vector3f f = (target - origin).normalized();
    Eigen::Vector3f s = f.cross(up).normalized();
    Eigen::Vector3f u = s.cross(f);

    Eigen::Matrix4f result = Eigen::Matrix4f::Identity();
    result(0, 0) = s(0);
    result(0, 1) = s(1);
    result(0, 2) = s(2);
    result(1, 0) = u(0);
    result(1, 1) = u(1);
    result(1, 2) = u(2);
    result(2, 0) = -f(0);
    result(2, 1) = -f(1);
    result(2, 2) = -f(2);
    result(0, 3) = -s.transpose() * origin;
    result(1, 3) = -u.transpose() * origin;
    result(2, 3) = f.transpose() * origin;
    return result;
}

Eigen::Matrix4f ortho(float left, float right, float bottom,
                      float top, float nearVal, float farVal) {
    Eigen::Matrix4f result = Eigen::Matrix4f::Identity();
    result(0, 0) = 2.0f / (right - left);
    result(1, 1) = 2.0f / (top - bottom);
    result(2, 2) = -2.0f / (farVal - nearVal);
    result(0, 3) = -(right + left) / (right - left);
    result(1, 3) = -(top + bottom) / (top - bottom);
    result(2, 3) = -(farVal + nearVal) / (farVal - nearVal);
    return result;
}

Eigen::Matrix4f frustum(float left, float right, float bottom,
                        float top, float nearVal,
                        float farVal) {
    Eigen::Matrix4f result = Eigen::Matrix4f::Zero();
    result(0, 0) = (2.0f * nearVal) / (right - left);
    result(1, 1) = (2.0f * nearVal) / (top - bottom);
    result(0, 2) = (right + left) / (right - left);
    result(1, 2) = (top + bottom) / (top - bottom);
    result(2, 2) = -(farVal + nearVal) / (farVal - nearVal);
    result(3, 2) = -1.0f;
    result(2, 3) = -(2.0f * farVal * nearVal) / (farVal - nearVal);
    return result;
}

Eigen::Matrix4f scale(const Eigen::Vector3f &v) {
    return Eigen::Affine3f(Eigen::Scaling(v)).matrix();
}

Eigen::Matrix4f translate(const Eigen::Vector3f &v) {
    return Eigen::Affine3f(Eigen::Translation<float, 3>(v)).matrix();
}

}  // namespace _
