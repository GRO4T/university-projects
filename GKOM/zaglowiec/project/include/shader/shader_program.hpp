#pragma once

#include <iostream>

#include "camera.hpp"
#include "opengl_all.hpp"

class ShaderProgram {
    GLuint program_id;  // The program ID
public:
    // Constructor reads and builds the shader
    ShaderProgram(const GLchar *vertex_path, const GLchar *fragment_path);
    virtual ~ShaderProgram() {}

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setViewMatrix(const Camera &camera) const;
    void setVec4(const std::string &name, const glm::vec4 vec) const;
    void setVec3(const std::string &name, const glm::vec3 vec) const;

    // Use the program
    void use() const { glUseProgram(getProgramID()); }

    // returns program ID
    GLuint getProgramID() const { return program_id; }

protected:
    void bindAttribute(int attrib_number, const std::string &attrib_name) const {
        glBindAttribLocation(program_id, attrib_number, attrib_name.c_str());
    }
};