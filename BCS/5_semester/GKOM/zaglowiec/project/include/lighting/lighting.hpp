#pragma once
#include <string>

#include "shader/shader_program.hpp"
#include "utils.hpp"

enum class LightType { Point, Direction, Spotlight };

struct SimpleLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    SimpleLight() {}
    SimpleLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : position(position), ambient(ambient), diffuse(diffuse), specular(specular) {}
    void setPosition(const glm::vec3& new_pos) { position = new_pos; }
    glm::vec3 getPosition() { return position; }
    glm::vec3 getAmbient() { return ambient; }
    glm::vec3 getDiffuse() { return diffuse; }
    glm::vec3 getSpecular() { return specular; }
    void setAmbient(const glm::vec3& val) { ambient = val; }
    void setDiffuse(const glm::vec3& val) { diffuse = val; }
    void setSpecular(const glm::vec3& val) { specular = val; }

    virtual void setShader(ShaderProgram shader) = 0;
    virtual void setShader(ShaderProgram shader, unsigned int index) = 0;
    virtual void updateDirection(glm::vec3 newDirection) = 0;
};

struct PointLight : SimpleLight {
    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
               float constant = 1.0f, float linear = 0.045, float quadratic = 0.0075) {
        this->position = position;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }
    float getLinear() { return linear; }
    float getConstant() { return constant; }
    float getQuadratic() { return quadratic; }

    void setShader(ShaderProgram shader) {
        shader.setVec3("pointLight.ambient", ambient);
        shader.setVec3("pointLight.diffuse", diffuse);
        shader.setVec3("pointLight.specular", specular);
        shader.setVec3("pointLight.position", position);
        shader.setFloat("pointLight.constant", constant);
        shader.setFloat("pointLight.linear", linear);
        shader.setFloat("pointLight.quadratic", quadratic);
    }
    void setShader(ShaderProgram shader, unsigned int index) {
        std::string set = "pointLights[" + std::to_string(index) + "]";
        shader.setVec3(set + ".ambient", ambient);
        shader.setVec3(set + ".diffuse", diffuse);
        shader.setVec3(set + ".specular", specular);
        shader.setVec3(set + ".position", position);
        shader.setFloat(set + ".constant", constant);
        shader.setFloat(set + ".linear", linear);
        shader.setFloat(set + ".quadratic", quadratic);
    }

private:
    void updateDirection(glm::vec3 newDirection){};
};

struct DirLight : SimpleLight {
    glm::vec3 direction;

    DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->direction = direction;
    }

    glm::vec3 getDirection() { return direction; }

    void setShader(ShaderProgram shader) {
        shader.setVec3("dirLight.direction", direction);
        shader.setVec3("dirLight.ambient", ambient);
        shader.setVec3("dirLight.diffuse", diffuse);
        shader.setVec3("dirLight.specular", specular);
    }

    void setShader(ShaderProgram shader, unsigned int index) {
        std::string set = "dirLights[" + std::to_string(index) + "]";
        shader.setVec3(set + ".direction", direction);
        shader.setVec3(set + ".ambient", ambient);
        shader.setVec3(set + ".diffuse", diffuse);
        shader.setVec3(set + ".specular", specular);
    }
    void updateDirection(glm::vec3 newDirection) { this->direction = newDirection; }

private:
    using SimpleLight::getPosition;
};

struct SpotLight : SimpleLight {
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
              glm::vec3 direction, float cutOff, float outerCutOff, float constant = 1.0f,
              float linear = 0.09f, float quadratic = 0.032f)
        : cutOff(cutOff), outerCutOff(outerCutOff) {
        this->position = position;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->direction = direction;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }
    glm::vec3 getDirection() { return direction; }
    float getCutOff() { return cutOff; }
    float getOuterCutOff() { return outerCutOff; }
    float getLinear() { return linear; }
    float getConstant() { return constant; }
    float getQuadratic() { return quadratic; }

    void setShader(ShaderProgram shader) {
        shader.setVec3("spotLight.ambient", ambient);
        shader.setVec3("spotLight.diffuse", diffuse);
        shader.setVec3("spotLight.specular", specular);
        shader.setVec3("spotLight.position", position);
        shader.setVec3("spotLight.direction", direction);
        shader.setFloat("spotLight.constant", constant);
        shader.setFloat("spotLight.linear", linear);
        shader.setFloat("spotLight.quadratic", quadratic);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutOff)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outerCutOff)));
    }
    void setShader(ShaderProgram shader, unsigned int index) {
        std::string set = "spotLights[" + std::to_string(index) + "]";
        shader.setVec3(set + ".ambient", ambient);
        shader.setVec3(set + ".diffuse", diffuse);
        shader.setVec3(set + ".specular", specular);
        shader.setVec3(set + ".position", position);
        shader.setVec3(set + ".direction", direction);
        shader.setFloat(set + ".constant", constant);
        shader.setFloat(set + ".linear", linear);
        shader.setFloat(set + ".quadratic", quadratic);
        shader.setFloat(set + ".cutOff", glm::cos(glm::radians(cutOff)));
        shader.setFloat(set + ".outerCutOff", glm::cos(glm::radians(outerCutOff)));
    }

    void updateDirection(glm::vec3 newDirection) { this->direction = newDirection; }
};

using LightPtr = std::unique_ptr<SimpleLight>;
using LightSharedPtr = std::shared_ptr<SimpleLight>;