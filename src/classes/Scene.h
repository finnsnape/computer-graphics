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
        RAY_TRACED_HARD,
        RAY_TRACED_PROXIMITY,
        RAY_TRACED_INCIDENCE
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
    void moveLight(Camera::Axis axis, int sign);
    void draw();
};