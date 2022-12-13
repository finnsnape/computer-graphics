#include "Light.h"

Light::Light() = default;
Light::Light(glm::vec3 _position, Mode _mode): position(_position), mode(_mode) {};