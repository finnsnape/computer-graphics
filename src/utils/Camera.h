#pragma once

#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <vector>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>

enum Axis { x, y, z };

class Camera {
private:
    float focalLength;
    glm::vec3 position;
    glm::mat3 orientation;
    glm::vec3 getTransposedPoint(glm::vec3 vertex);
    glm::vec3 getRaySolution(ModelTriangle triangle, glm::vec3 rayDirection);
    static glm::vec3 getRayIntersection(ModelTriangle triangle, glm::vec3 possibleSolution);
public:
    std::vector<std::vector<float>> depthBuffer;
    Camera(glm::vec3 position, float focalLength);
    void resetDepthBuffer();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
    CanvasPoint getIntersectionPoint(glm::vec3 vertex);
    void orbit(Axis axis, int sign, glm::vec3 vertex);
    glm::vec3 getRayDirection(CanvasPoint canvasPoint);
    static bool validateRaySolution(glm::vec3 possibleSolution);
    RayTriangleIntersection getClosestIntersection(std::vector<ModelTriangle> triangles, glm::vec3 rayDirection);
};
