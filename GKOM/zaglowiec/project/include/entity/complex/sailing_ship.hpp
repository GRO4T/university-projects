#pragma once

#include <array>
#include <cmath>

#include "complex_entity.hpp"
#include "torch.hpp"
#include "lighting/lighting.hpp"

class SailingShip : public ComplexEntity {
public:
    SailingShip(const Transform& t);

    void animate(float delta_time) override;
    void setStationary(bool val) { stationary = val; }
    glm::vec3 getFront() const { return front; }

private:
    constexpr static float DECK_DIAMETER = 10.0f;
    constexpr static float DECK_LENGTH = 20.0f;

    const std::string WOOD_TEXTURE_PATH = "res/wood.jpg";
    const std::string LINEN_TEXTURE_PATH = "res/linen.jpeg";

    const float SHIP_SPEED = 0.02f;
    const float SHIP_STEERING_SPEED = 0.02f;
    const float LEAN_MULT = 0.025f;

    class RailingsGenerator {
    public:
        RailingsGenerator();
        void generate(EntitySPtr root, TextureSPtr texture);
    private:
        const float EPSILON = 0.001f;
        const std::array<std::pair<int, int>, 4> MIRROR_COORDS{{{1, 1}, {1, -1}, {-1, -1}, {-1, 1}}};

        struct RailingsDimensions {
            const float POLE_WIDTH = 0.25f;
            const float POLE_HEIGHT = 1.5f;
            const int NUM_POLES_X = 5;
            const int NUM_POLES_Y = 11;
            const float PADDING = 0.25f;
        } rd;

        void generateVerticalPoles(EntitySPtr root, TextureSPtr texture);
        void generateHorizontalPoles(EntitySPtr root, TextureSPtr texture);

        float max_pole_x, max_pole_y;

        float x_offset;
        float y_offset;
        float z_offset;
    };

    void generateMast();
    void generateSteeringWheel();
    void generateTorches();

    void handleMovement(float delta_time);

    TextureSPtr wood_texture;
    MaterialSPtr material;

    std::vector<Torch> torches;
    std::vector<LightSharedPtr> lights;

    EntitySPtr steering_wheel;

    bool stationary = true;
    glm::vec3 front = glm::vec3(0, 0, 1);
};