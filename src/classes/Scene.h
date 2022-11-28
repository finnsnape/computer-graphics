#pragma once

#include <glm/glm.hpp>
#include "Camera.h"
#include <DrawingWindow.h>
#include <ModelTriangle.h>

enum Mode {RASTERISED, WIRE_FRAME, RAY_TRACED};

class Scene {
public:
    float width;
    float height;
    std::vector<ModelTriangle> triangles;
    Camera camera;
    DrawingWindow window;
    glm::vec3 lightSource;
    Scene(float width, float height, std::vector<ModelTriangle> triangles, Camera camera);
    Scene(float width, float height, std::vector<ModelTriangle> triangles, Camera camera, glm::vec3 lightSource);
    void draw(Mode mode);
};