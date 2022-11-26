#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "TriangleUtils.h"
#include "FilesUtils.h"
#include "RayTracingUtils.h"

#define WIDTH 480
#define HEIGHT 480

void drawRayTraced(Scene &scene) {
    scene.window.clearPixels();
    for (int x=0; x<scene.width; x++) {
        for (int y=0; y<scene.height; y++) {
            CanvasPoint canvasPoint((float) x, (float) y);
            RayTriangleIntersection closestTriangle = RayTracingUtils::findClosestTriangle(scene, canvasPoint);
            if (closestTriangle.distanceFromCamera != FLT_MAX) {
                drawPixel(scene.window, canvasPoint, closestTriangle.intersectedTriangle.colour);
            }
        }
    }
}

void handleEvent(SDL_Event event) {
    //std::cout << event.type << std::endl;
}

int main(int argc, char *argv[]) {
    float scaleFactor = 0.35;
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    float focalLength = 2.0;
    Camera camera(WIDTH, HEIGHT, focalLength, initialPosition);
    std::vector<ModelTriangle> triangles = loadOBJ(scaleFactor, "cornell-box.obj");
    Scene scene(WIDTH, HEIGHT, triangles, camera);
    SDL_Event event;
    while(true) {
        if (scene.window.pollForInputEvents(event)) handleEvent(event);
        drawRayTraced(scene);
        scene.window.renderFrame();
    }
}
