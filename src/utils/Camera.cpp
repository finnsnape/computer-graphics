#include "Camera.h"

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

Camera::Camera(glm::vec3 p, glm::mat3 o, float f): position(p), orientation(o), focalLength(f) {}

glm::mat4 Camera::buildMatrix() {
    glm::mat4 matrix(
    orientation[0][0], orientation[0][1], orientation[0][2], 0.0,
    orientation[1][0], orientation[1][1], orientation[1][2], 0.0,
    orientation[2][0], orientation[2][1], orientation[2][2], 0.0,
    position.x, position.y, position.z, 1.0
    );
    return matrix;
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
    this->orientation = transformationMatrix * this->orientation;
}

void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 temp(0.0, 1.0, 0.0); // pointing upwards
    //vertex = this->getTransposedPoint(vertex);
    glm::vec3 forward = glm::normalize(vertex - this->position);
    glm::vec3 right = glm::normalize(glm::cross(forward, temp));
    glm::vec3 up = glm::cross(right, forward);
    right = -right;
    this->orientation = glm::mat3(
    right.x, up.x, forward.x,
    right.y, up.y, forward.y,
    right.z, up.z, forward.z
    );
    this->position = {glm::dot(right, this->position), -glm::dot(up, this->position), -glm::dot(forward, this->position)};
}

glm::vec3 Camera::getTransposedPoint(glm::vec3 vertex) {
    glm::mat4 cameraToWorld = this->buildMatrix();
    glm::mat4 worldToCamera = glm::inverse(cameraToWorld);
    glm::vec4 tempVertex(vertex.x, vertex.y, vertex.z, 1.0);
    glm::vec4 transposedVertex = worldToCamera * tempVertex;
    return {transposedVertex.x, transposedVertex.y, transposedVertex.z};
}

CanvasPoint Camera::getIntersectionPoint(glm::vec3 vertex) {
    glm::vec3 transposedVertex = this->getTransposedPoint(vertex);
    float u = this->focalLength * (WIDTH/2) * (transposedVertex.x/(-1*transposedVertex.z)) + WIDTH/2;
    float v = this->focalLength * (HEIGHT/2) * (transposedVertex.y/(-1*transposedVertex.z)) + HEIGHT/2;
    return {u, v, transposedVertex.z};
}