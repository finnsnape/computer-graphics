#include "Camera.h"
#include <cmath>

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

Camera::Camera(glm::vec3 _position, float _focalLength): focalLength(_focalLength), position(_position) {
    this->lookAt({0.0, 0.0, 0.0});
    this->resetDepthBuffer();
}

void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(HEIGHT, std::vector<float>(WIDTH, 0.0));
}

void Camera::translate(Axis axis, int sign) {
    float modifier = 0.03 * sign;
    switch(axis) {
        case x:
            this->position.x += modifier;
            break;
        case y:
            this->position.y += modifier;
            break;
        case z:
            this->position.z += modifier;
            break;
        default:
            break;
    }
}

void Camera::rotate(Axis axis, int sign) {
    // anti-clockwise
    double modifier = 0.03 * sign;
    glm::mat3 transformationMatrix;
    switch(axis) {
        case x:
            transformationMatrix = glm::mat3(
            1.0, 0.0, 0.0,
            0.0, cos(modifier), sin(modifier),
            0.0, -sin(modifier), cos(modifier)
            );
            break;
        case y:
            transformationMatrix = glm::mat3(
            cos(modifier), 0.0, -sin(modifier),
            0.0, 1.0, 0.0,
            sin(modifier), 0.0, cos(modifier)
            );
            break;
        default:
            break;
    }
    this->position = transformationMatrix * this->position;
}

void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 forward = glm::normalize(this->position - vertex);
    glm::vec3 direction(0.0, 1.0, 0.0);
    glm::vec3 right = glm::cross(forward, direction);
    glm::vec3 up = glm::cross(forward, right);
    this->orientation = {right, up, forward};
}

glm::vec3 Camera::getTransposedPoint(glm::vec3 vertex) const {
    glm::vec3 vertexDifference = vertex - this->position;
    glm::vec3 transposedVertex = vertexDifference * this->orientation;
    return {transposedVertex.x, transposedVertex.y, -transposedVertex.z};
}

CanvasPoint Camera::getIntersectionPoint(glm::vec3 vertex) {
    glm::vec3 transposedVertex = this->getTransposedPoint(vertex);
    float u = this->focalLength * (WIDTH/2) * (transposedVertex.x/(transposedVertex.z)) + WIDTH/2;
    float v = this->focalLength * (HEIGHT/2) * (transposedVertex.y/(transposedVertex.z)) + HEIGHT/2;
    return {u, v, transposedVertex.z};
}

void Camera::orbit(Axis axis, int sign, glm::vec3 vertex) {
    this->rotate(axis, sign);
    this->lookAt(vertex);
}
