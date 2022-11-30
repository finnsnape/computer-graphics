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
#include "EventUtils.h"

#define WIDTH 240
#define HEIGHT 240

void printInstructions() {
    std::cout <<
    "=======================" << std::endl <<
    "Modes: " << std::endl <<
    "1: Wire Frame" << std::endl <<
    "2: Rasterised" << std::endl <<
    "3: Ray Traced" << std::endl <<
    "4: Ray Traced (Shadows)" << std::endl <<
    "=======================" << std::endl;
}

Scene init(Mode mode, float scaleFactor, float focalLength, glm::vec3 initialPosition, glm::vec3 lightSource) {
    std::string objFileName = "cornell-box.obj";
    std::string mtlFileName = "cornell-box.mtl";
    Camera camera(WIDTH, HEIGHT, focalLength, initialPosition);
    std::vector<ModelTriangle> triangles = FilesUtils::loadOBJ(scaleFactor, objFileName, mtlFileName);
    Scene scene(WIDTH, HEIGHT, mode, triangles, camera, lightSource);
    return scene;
}

int main(int argc, char *argv[]) {
    printInstructions();
    // TODO: clean this up, maybe just do scene.lightSource = ... etc.
    Mode mode = WIRE_FRAME; // RASTERISED, RAY_TRACED, WIRE_FRAME, RAY_TRACED_SHADOWS
    float scaleFactor = 0.35;
    float focalLength = 2.0;
    glm::vec3 lightSource(0.0, 0.35, 0.0);
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    Scene scene = init(mode, scaleFactor, focalLength, initialPosition, lightSource);
    SDL_Event event;
    while(true) {
        if (scene.window.pollForInputEvents(event)) EventUtils::handleEvent(event, scene);
        scene.draw();
        scene.window.renderFrame();
    }
}