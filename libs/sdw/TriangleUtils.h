#pragma once

#include <vector>
#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "DrawingWindow.h"
#include "Colour.h"

void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour);
void drawLine(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour);
float getClockwiseAngle(CanvasPoint point);
bool comparePoints(CanvasPoint point1, CanvasPoint point2);
float area(int x1, int y1, int x2, int y2, int x3, int y3);
bool isInTriangle(CanvasTriangle triangle, CanvasPoint testPoint);
std::vector<float> boundingBox(CanvasTriangle triangle);
void drawFilledTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour);