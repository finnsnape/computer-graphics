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
    "4: Default" << std::endl <<
    "5: Hard Shadows" << std::endl <<
    "6: Proximity" << std::endl <<
    "7: Angle of Incidence" << std::endl <<
    "8: Ambient" << std::endl <<
    "9: Specular" << std::endl <<
    "0: Mirror" << std::endl <<
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
    "L: Look At (0, 0, 0)" << std::endl <<
    "O: Orbit (y, positive)" << std::endl <<
    "W: Translate (y, positive)" << std::endl <<
    "S: Translate (y, negative)" << std::endl <<
    "A: Translate (x, negative)" << std::endl <<
    "D: Translate (x, positive)" << std::endl <<
    "-: Translate (z, positive)" << std::endl <<
    "=: Translate (z, negative)" << std::endl <<
    "UP: Rotate (x, positive)" << std::endl <<
    "DOWN: Rotate (x, negative)" << std::endl <<
    "LEFT: Rotate (y, positive)" << std::endl <<
    "RIGHT: Rotate (y, negative)" << std::endl <<
    std::endl <<
    "MISC: " << std::endl <<
    "F: Save as file" << std::endl <<
    "=================================" << std::endl;
}

Scene initScene(Scene::RenderMode renderMode, Light light, glm::vec3 initialPosition) {
    std::string objFileName = "cornell-box.obj";
    std::string mtlFileName = "cornell-box.mtl";
    Camera camera(WIDTH, HEIGHT, initialPosition, false);
    std::vector<ModelTriangle> triangles = FilesUtils::loadOBJ(objFileName, mtlFileName);
    Scene scene(WIDTH, HEIGHT, renderMode, light, triangles, camera);
    return scene;
}

int main(int argc, char *argv[]) {
    Scene::RenderMode renderMode = Scene::RAY_TRACED;
    Light::Mode lightMode = Light::SPECULAR;
    glm::vec3 lightSource(0.f, 0.9f, 0.f);
    //glm::vec3 lightSource(0.8, 0.8, -0.8);
    //glm::vec3 lightSource(0.0, 0.55, 0.7);
    Light light(lightSource, lightMode);
    glm::vec3 initialPosition(0.f, 0.f, 4.f);
    Scene scene = initScene(renderMode, light, initialPosition);
    SDL_Event event;
    printInstructions();
    scene.draw();
    while (true) {
        if (scene.window.pollForInputEvents(event)) EventUtils::handleEvent(event, scene);
        if (scene.camera.orbit) {
            scene.camera.rotate(Camera::Axis::y, 1.f);
            scene.draw();
        }
        scene.window.renderFrame();
    }
}