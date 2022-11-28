#pragma once

#include <glm/glm.hpp>
#include "Camera.h"
#include "Scene.h"
#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <CanvasPoint.h>

namespace RasterisingUtils {
    CanvasTriangle makeCanvasTriangle(Scene &scene, ModelTriangle triangle);
}