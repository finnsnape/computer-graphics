#pragma once

#include <glm/glm.hpp>

struct Light {
    enum Mode {
        DEFAULT,
        PROXIMITY,
        SPECULAR
    };
    glm::vec3 position;
    Mode mode;
    Light();
    Light(glm::vec3 position, Mode mode);
};