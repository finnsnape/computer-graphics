#pragma once

#include <glm/glm.hpp>

struct Light {
    enum Mode {
        DEFAULT,
        HARD_SHADOWS,
        PROXIMITY,
        ANGLE_OF_INCIDENCE,
        AMBIENT,
        SPECULAR
    };
    glm::vec3 position;
    Mode mode;
    Light();
    Light(glm::vec3 position, Mode mode);
};