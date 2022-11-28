#pragma once

#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <vector>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>

enum Axis { x, y, z };

class Camera {
private:
    float width;
    float height;
public:
    float focalLength;
    glm::vec3 position;
    glm::mat3 orientation;
    std::vector<std::vector<float>> depthBuffer;
    Camera(float width, float height, float focalLength, glm::vec3 position);
    void resetDepthBuffer();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
    void orbit(Axis axis, int sign, glm::vec3 vertex);
};
