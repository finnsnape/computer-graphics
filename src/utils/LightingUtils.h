#pragma once

#include <RayTriangleIntersection.h>

class Scene;

namespace ShadowUtils {
    void applyShadows(Scene &scene, RayTriangleIntersection &closestTriangle);
}