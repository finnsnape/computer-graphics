#pragma once

#include <glm/glm.hpp>

struct Light {
    enum Mode {
        DEFAULT,
        PROXIMITY,
        PHONG
    };
    glm::vec3 position;
    Mode mode;
    float ambientIntensity;
    glm::vec3 colour;
    Light();
    Light(glm::vec3 position, Mode mode, float ambientIntensity, glm::vec3 colour);
};