#pragma once

#include <glm/glm.hpp>
#include <CanvasPoint.h>

class Scene; // pre-declare to avoid circular dependency

namespace RayTracingUtils {
    void draw(Scene &scene);
}