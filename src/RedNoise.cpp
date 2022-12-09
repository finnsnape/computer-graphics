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
    "RENDER MODES: " << std::endl <<
    "1: Wire Frame" << std::endl <<
    "2: Rasterised" << std::endl <<
    "3: Ray Traced" << std::endl <<
    std::endl <<
    "LIGHTING MODES (Ray Traced): " << std::endl <<
    "4: Hard Shadows" << std::endl <<
    "5: Proximity" << std::endl <<
    "6: Angle of Incidence" << std::endl <<
    std::endl <<
    "LIGHT POSITION OPERATIONS: " << std::endl <<
    "H: Translate (y, positive)" << std::endl <<
    "N: Translate (y, negative)" << std::endl <<
    "B: Translate (x, negative)" << std::endl <<
    "M: Translate (x, positive)" << std::endl <<
    "J: Translate (z, positive)" << std::endl <<
    "K: Translate (z, negative)" << std::endl <<
    std::endl <<
    "CAMERA POSITION OPERATIONS: " << std::endl <<
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

Scene initScene(Scene::RenderMode renderMode, Scene::LightingMode lightingMode, float scaleFactor, glm::vec3 initialPosition, glm::vec3 lightSource) {
    std::string objFileName = "cornell-box.obj";
    std::string mtlFileName = "cornell-box.mtl";
    Camera camera(WIDTH, HEIGHT, initialPosition, false);
    std::vector<ModelTriangle> triangles = FilesUtils::loadOBJ(scaleFactor, objFileName, mtlFileName);
    Scene scene(WIDTH, HEIGHT, renderMode, lightingMode, triangles, camera, lightSource);
    return scene;
}

int main(int argc, char *argv[]) {
    Scene::RenderMode renderMode = Scene::RAY_TRACED;
    Scene::LightingMode lightingMode = Scene::ANGLE_OF_INCIDENCE;
    float scaleFactor = 0.35;
    glm::vec3 lightSource(0.0, 0.9, 0.7);
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    Camera camera(WIDTH, HEIGHT, initialPosition, false);
    Scene scene = initScene(renderMode, lightingMode, scaleFactor, initialPosition, lightSource);
    SDL_Event event;
    printInstructions();
    scene.draw();
    while(true) {
        if (scene.window.pollForInputEvents(event)) EventUtils::handleEvent(event, scene);
        scene.window.renderFrame();
    }
}