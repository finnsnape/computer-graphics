#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "FilesUtils.h"
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

Scene initScene(Mode mode, float scaleFactor, float focalLength, glm::vec3 initialPosition, glm::vec3 lightSource) {
    std::string objFileName = "cornell-box.obj";
    std::string mtlFileName = "cornell-box.mtl";
    Camera camera(WIDTH, HEIGHT, focalLength, initialPosition);
    std::vector<ModelTriangle> triangles = FilesUtils::loadOBJ(scaleFactor, objFileName, mtlFileName);
    Scene scene(WIDTH, HEIGHT, mode, triangles, camera, lightSource);
    return scene;
}

int main(int argc, char *argv[]) {
    Mode mode = WIRE_FRAME;
    float scaleFactor = 0.35;
    float focalLength = 2.0;
    glm::vec3 lightSource(0.0, 0.35, 0.0);
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    Scene scene = initScene(mode, scaleFactor, focalLength, initialPosition, lightSource);
    SDL_Event event;
    printInstructions();
    while(true) {
        if (scene.window.pollForInputEvents(event)) EventUtils::handleEvent(event, scene);
        scene.draw();
        scene.window.renderFrame();
    }
}