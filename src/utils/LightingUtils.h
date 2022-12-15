#pragma once

#include <RayTriangleIntersection.h>

class Scene;

namespace LightingUtils {
    Colour applyLighting(Scene &scene, RayTriangleIntersection &closestTriangle);
}