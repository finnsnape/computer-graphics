#pragma once

#include <vector>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>

namespace TriangleUtils {
    void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour);
    void drawFilledTriangle(DrawingWindow &window, std::vector<std::vector<float>> &depthBuffer, CanvasTriangle triangle, Colour colour);
}