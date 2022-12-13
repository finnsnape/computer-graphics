#pragma once

#include <glm/glm.hpp>
#include <RayTriangleIntersection.h>
#include <Ray.h>

class Scene; // pre-declare to avoid circular dependency

namespace RayTracingUtils {
    RayTriangleIntersection findClosestTriangle(Scene &scene, Ray ray, bool mirror, int k);
    bool canSeeLight(Scene &scene, const RayTriangleIntersection &closestTriangle);
    void draw(Scene &scene);
}