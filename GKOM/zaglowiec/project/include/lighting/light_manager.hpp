#pragma once
#include <vector>

#include "lighting/lighting.hpp"
#include "shader/shader_program.hpp"

class LightManager {
public:
    static LightManager &getInstance() {
        static LightManager instance;
        return instance;
    }
    void addSpotlight(LightSharedPtr spotlight) { spotlights.push_back(spotlight); }
    void addPointLight(LightSharedPtr pointlight) { pointlights.push_back(pointlight); }
    void addDirlight(LightSharedPtr dirlight) { dirlights.push_back(dirlight); }

    void setShader(ShaderProgram shader) {
        if (spotlights.size() > 1) {
            for (int i = 0; i < spotlights.size(); i++) {
                spotlights[i]->setShader(shader, i);
            }
        } else if (spotlights.size() == 1) {
            spotlights[0]->setShader(shader);
        }

        if (pointlights.size() > 1) {
            for (int i = 0; i < pointlights.size(); i++) {
                pointlights[i]->setShader(shader, i);
            }
        } else if (pointlights.size() == 1) {
            pointlights[0]->setShader(shader);
        }

        if (dirlights.size() > 1) {
            for (int i = 0; i < dirlights.size(); i++) {
                dirlights[i]->setShader(shader, i);
            }
        } else if (dirlights.size() == 1) {
            dirlights[0]->setShader(shader);
        }
    }
    std::vector<LightSharedPtr> getSpotlights() { return spotlights; }
    std::vector<LightSharedPtr> getDirlights() { return dirlights; }
    std::vector<LightSharedPtr> getPointLights() { return pointlights; }

private:
    LightManager() = default;
    LightManager(const LightManager &) = delete;
    LightManager(LightManager &&) = delete;
    LightManager &operator=(LightManager &&) = delete;
    LightManager &operator=(const LightManager &) = delete;
    std::vector<LightSharedPtr> spotlights;
    std::vector<LightSharedPtr> pointlights;
    std::vector<LightSharedPtr> dirlights;
};
