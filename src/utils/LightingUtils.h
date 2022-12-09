#pragma once

#include <RayTriangleIntersection.h>

class Scene;

namespace LightingUtils {
    void applyLighting(Scene &scene, RayTriangleIntersection &closestTriangle);
}