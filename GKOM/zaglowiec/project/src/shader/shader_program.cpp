#include "shader/shader_program.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "opengl_all.hpp"
#include "utils.hpp"

std::string read_shader_code(const GLchar *shader_path) {
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::badbit);

    shader_file.open(shader_path);
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    return shader_stream.str();
}

GLuint compile_shader(const GLchar *shader_code, GLenum shader_type) {
    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);

    // Print compile errors if any
    GLint success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader_id, sizeof(infoLog), NULL, infoLog);
        std::string msg = std::string("Shader compilation: ") + infoLog;
        throw std::runtime_error(msg.c_str());
    }
    return shader_id;
}

ShaderProgram::ShaderProgram(const GLchar *vertex_path, const GLchar *fragment_path) {
    std::cout << "Compiling vertex shader: " + std::string(vertex_path) << std::endl;
    std::string vertex_code = read_shader_code(vertex_path);
    GLuint vertex_id = compile_shader(vertex_code.c_str(), GL_VERTEX_SHADER);

    std::cout << "Compiling fragment shader: " + std::string(fragment_path) << std::endl;
    std::string fragment_code = read_shader_code(fragment_path);
    GLuint fragment_id = compile_shader(fragment_code.c_str(), GL_FRAGMENT_SHADER);

    // link shader program
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    glLinkProgram(program_id);

    // Print linking errors if any
    GLint success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program_id, sizeof(infoLog), NULL, infoLog);
        std::string msg = std::string("Shader program linking: ") + infoLog;
        throw std::runtime_error(msg.c_str());
    }

    // Delete the shaders as they're linked into our program now and no longer
    // necessery
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}

void ShaderProgram::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(getProgramID(), name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(getProgramID(), name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(getProgramID(), name.c_str()), value);
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(getProgramID(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setViewMatrix(const Camera &camera) const {
    glm::mat4 view;
    utils::createViewMatrix(view, camera);
    setMat4("View", view);
}

void ShaderProgram::setVec4(const std::string &name, const glm::vec4 vec) const {
    glUniform4f(glGetUniformLocation(getProgramID(), name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(getProgramID(), name.c_str()), vec.x, vec.y, vec.z);
}
