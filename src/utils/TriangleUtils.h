#pragma once

#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>

class Scene; // pre-declare to avoid circular dependency

namespace TriangleUtils {
    bool isInsideCanvas(DrawingWindow &window, CanvasPoint point);
    void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour);
    void drawStrokedTriangle(Scene &scene, CanvasTriangle triangle);
    void drawFilledTriangle(Scene &scene, CanvasTriangle triangle, Colour colour);
}