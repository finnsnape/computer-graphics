#pragma once

#include <glm/glm.hpp>
#include "Camera.h"
#include <DrawingWindow.h>
#include <ModelTriangle.h>

class Scene {
public:
    enum Mode {
        WIRE_FRAME,
        RASTERISED,
        RAY_TRACED,
        RAY_TRACED_SHADOWS
    };
    float width;
    float height;
    Mode mode;
    std::vector<ModelTriangle> triangles;
    Camera camera;
    DrawingWindow window;
    glm::vec3 lightSource;
    Scene(float width, float height, Mode mode, std::vector<ModelTriangle> triangles, Camera camera);
    Scene(float width, float height, Mode mode, std::vector<ModelTriangle> triangles, Camera camera, glm::vec3 lightSource);
    void draw();
};