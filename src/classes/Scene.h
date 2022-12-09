#pragma once

#include <glm/glm.hpp>
#include "Camera.h"
#include <DrawingWindow.h>
#include <ModelTriangle.h>

class Scene {
public:
    enum RenderMode {
        WIRE_FRAME,
        RASTERISED,
        RAY_TRACED,
    };
    enum LightingMode {
        DEFAULT,
        HARD_SHADOWS,
        PROXIMITY,
        ANGLE_OF_INCIDENCE,
        SPECULAR,
        AMBIENT
    };
    float width;
    float height;
    RenderMode renderMode;
    LightingMode lightingMode;
    std::vector<ModelTriangle> triangles;
    Camera camera;
    DrawingWindow window;
    glm::vec3 lightSource;
    Scene(float width, float height, RenderMode renderMode, LightingMode lightingMode, std::vector<ModelTriangle> triangles, Camera camera, glm::vec3 lightSource);
    void moveLight(Camera::Axis axis, int sign);
    void draw();
};