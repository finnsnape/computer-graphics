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
#include "RasterisingUtils.h"

#define WIDTH 240
#define HEIGHT 240

void handleEvent(SDL_Event event) {
    //std::cout << event.type << std::endl;
}

Scene init(Mode mode, float scaleFactor, float focalLength, glm::vec3 initialPosition, glm::vec3 lightSource) {
    std::string objFileName = "cornell-box.obj";
    std::string mtlFileName = "cornell-box.mtl";
    Camera camera(WIDTH, HEIGHT, focalLength, initialPosition);
    std::vector<ModelTriangle> triangles = FilesUtils::loadOBJ(scaleFactor, objFileName, mtlFileName);
    Scene scene(WIDTH, HEIGHT, triangles, camera, lightSource);
    return scene;
}

int main(int argc, char *argv[]) {
    Mode mode = RAY_TRACED_SHADOWS; // RASTERISED, RAY_TRACED, WIRE_FRAME, RAY_TRACED_SHADOWS
    float scaleFactor = 0.35;
    float focalLength = 2.0;
    glm::vec3 lightSource(0.0, 0.35, 0.0);
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    Scene scene = init(mode, scaleFactor, focalLength, initialPosition, lightSource);
    SDL_Event event;
    while(true) {
        if (scene.window.pollForInputEvents(event)) handleEvent(event);
        scene.draw(mode);
        scene.window.renderFrame();
    }
}