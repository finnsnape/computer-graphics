#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>
#include "Camera.h"
#include "Scene.h"

namespace RayTracingUtils {
    RayTriangleIntersection findClosestTriangle(Scene &scene, CanvasPoint canvasPoint);
}