#include "Triangles.h"
#include <algorithm>

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

/* Drawing */

void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour) {
    int x = round(point.x);
    int y = round(point.y);
    uint32_t colourCode = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
    window.setPixelColour(x, y, colourCode);
}

/* Triangles */

float calculatePointDepth(CanvasTriangle triangle, CanvasPoint point) {
    float denominator = (triangle.v1().y-triangle.v2().y)*(triangle.v0().x-triangle.v2().x)+(triangle.v2().x-triangle.v1().x)*(triangle.v0().y-triangle.v2().y);
    float w0 = ((triangle.v1().y-triangle.v2().y)*(point.x-triangle.v2().x)+(triangle.v2().x-triangle.v1().x)*(point.y-triangle.v2().y))/denominator;
    float w1 = ((triangle.v2().y-triangle.v0().y)*(point.x-triangle.v2().x)+(triangle.v0().x-triangle.v2().x)*(point.y-triangle.v2().y))/denominator;
    float w2 = 1.0 - w0 - w1;
    if (w0 < 0 || w1 < 0 || w2 < 0) {
        return 0; // point is not inside the triangle
    }
    return 1/((w0 * triangle.v0().depth) + (w1 * triangle.v1().depth) + (w2 * triangle.v2().depth));
}

std::vector<float> boundingBox(CanvasTriangle triangle) {
    float minX = std::min({triangle.v0().x, triangle.v1().x, triangle.v2().x});
    float maxX = std::max({triangle.v0().x, triangle.v1().x, triangle.v2().x});
    float minY = std::min({triangle.v0().y, triangle.v1().y, triangle.v2().y});
    float maxY = std::max({triangle.v0().y, triangle.v1().y, triangle.v2().y});
    //std::cout << minX << "-" << maxX << " " << minY << "-" << maxY << std::endl;
    return {minX, maxX, minY, maxY};
}

void drawFilledTriangle(DrawingWindow &window, std::vector<std::vector<float>> &depthBuffer, CanvasTriangle triangle, Colour colour) {
    // we should be getting the smallest possible bounding box for the triangle rather than iterating over all of them
    std::vector<float> boundedBy = boundingBox(triangle);
    for (int x=boundedBy[0]; x<boundedBy[1]; x++) {
        for (int y=boundedBy[2]; y<boundedBy[3]; y++) {
            float depth = calculatePointDepth(triangle, CanvasPoint(x, y));
            if (depth == 0) {
                continue; // point is outside triangle
            }
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
                continue; // point is outside frame
            }
            if (depth < depthBuffer[x][y]) {
                continue; // something in front of our pixel has already been placed
            }
            drawPixel(window, CanvasPoint(x, y), colour);
            depthBuffer[x][y] = depth;
        }
    }
}