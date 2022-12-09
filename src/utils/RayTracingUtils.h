#pragma once

#include <glm/glm.hpp>
#include <RayTriangleIntersection.h>

class Scene; // pre-declare to avoid circular dependency

namespace RayTracingUtils {
    bool canSeeLight(Scene &scene, const RayTriangleIntersection &closestTriangle);
    void draw(Scene &scene);
}