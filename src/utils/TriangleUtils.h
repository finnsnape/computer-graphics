#pragma once

#include <vector>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>
#include "Scene.h"
#include <cmath>

namespace TriangleUtils {
    void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour);
    void drawFilledTriangle(Scene &scene, CanvasTriangle triangle, Colour colour);
}