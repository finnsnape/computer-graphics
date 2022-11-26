#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <vector>
#include "Triangles.h"
#include "Files.h"
#include "Camera.h"

#define WIDTH 240
#define HEIGHT 240

void drawRayTraced(DrawingWindow &window, Camera &camera, std::vector<ModelTriangle> &modelTriangles) {
    window.clearPixels();
    for (int x=0; x<WIDTH; x++) {
        for (int y=0; y<HEIGHT; y++) {
            CanvasPoint canvasPoint((float) x, (float) y);
            glm::vec3 rayDirection = camera.getRayDirection(canvasPoint);
            RayTriangleIntersection solution = camera.getClosestIntersection(modelTriangles, rayDirection);
            if (solution.exists) {
                drawPixel(window, canvasPoint, solution.intersectedTriangle.colour);
            }
        }
    }
}

void handleEvent(SDL_Event event) {
    std::cout << event.type << std::endl;
}

int main(int argc, char *argv[]) {
    float scaleFactor = 0.35;
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    float focalLength = 2.0;
    Camera camera(initialPosition, focalLength);
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    std::vector<ModelTriangle> modelTriangles = loadOBJ(scaleFactor, "cornell-box.obj");
    while(true) {
        if (window.pollForInputEvents(event)) handleEvent(event);
        drawRayTraced(window, camera, modelTriangles);
        window.renderFrame();
    }
}
