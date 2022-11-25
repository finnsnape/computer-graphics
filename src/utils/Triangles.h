#pragma once

#include <vector>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <CanvasTriangle.h>
#include <Colour.h>

void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour);
float calculatePointDepth(CanvasTriangle triangle, CanvasPoint point);
std::vector<float> boundingBox(CanvasTriangle triangle);
void drawFilledTriangle(DrawingWindow &window, std::vector<std::vector<float>> &depthBuffer, CanvasTriangle triangle, Colour colour);