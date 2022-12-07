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
    "=================================" << std::endl <<
    "MODES: " << std::endl <<
    "1: Wire Frame" << std::endl <<
    "2: Rasterised" << std::endl <<
    "3: Ray Traced" << std::endl <<
    "4: Ray Traced (Hard Shadows)" << std::endl <<
    "5: Ray Traced (Proximity Lighting)" << std::endl <<
    "6: Ray Traced (Angle of Incidence Lighting)" << std::endl <<
    std::endl <<
    "LIGHT OPERATIONS: " << std::endl <<
    "H: Translate (y, positive)" << std::endl <<
    "N: Translate (y, negative)" << std::endl <<
    "B: Translate (x, negative)" << std::endl <<
    "M: Translate (x, positive)" << std::endl <<
    "J: Translate (z, positive)" << std::endl <<
    "K: Translate (z, negative)" << std::endl <<
    std::endl <<
    "CAMERA OPERATIONS: " << std::endl <<
    "O: Orbit (y, positive)" << std::endl <<
    "W: Translate (y, positive)" << std::endl <<
    "S: Translate (y, negative)" << std::endl <<
    "A: Translate (x, negative)" << std::endl <<
    "D: Translate (x, positive)" << std::endl <<
    "-: Translate (z, positive)" << std::endl <<
    "=: Translate (z, negative)" << std::endl <<
    "UP: Rotate (x, negative)" << std::endl <<
    "DOWN: Rotate (x, positive)" << std::endl <<
    "LEFT: Rotate (y, negative)" << std::endl <<
    "RIGHT: Rotate (y, positive)" << std::endl <<
    "=================================" << std::endl;
}

Scene initScene(Scene::Mode mode, float scaleFactor, glm::vec3 initialPosition, glm::vec3 lightSource) {
    std::string objFileName = "cornell-box.obj";
    std::string mtlFileName = "cornell-box.mtl";
    Camera camera(WIDTH, HEIGHT, initialPosition, false);
    std::vector<ModelTriangle> triangles = FilesUtils::loadOBJ(scaleFactor, objFileName, mtlFileName);
    Scene scene(WIDTH, HEIGHT, mode, triangles, camera, lightSource);
    return scene;
}

int main(int argc, char *argv[]) {
    Scene::Mode mode = Scene::RASTERISED;
    float scaleFactor = 0.35;
    glm::vec3 lightSource(0.0, 0.8, 0.0);
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    Camera camera(WIDTH, HEIGHT, initialPosition, false);
    Scene scene = initScene(mode, scaleFactor, initialPosition, lightSource);
    SDL_Event event;
    printInstructions();
    scene.draw();
    while(true) {
        if (scene.window.pollForInputEvents(event)) EventUtils::handleEvent(event, scene);
        scene.window.renderFrame();
    }
}