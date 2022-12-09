#include "Camera.h"

#include <CanvasPoint.h>


Camera::Camera(float _width, float _height, glm::vec3 _position, bool _orbit): width(_width), height(_height), position(_position), orbit(_orbit) {
    this->lookAt({0.0, 0.0, 0.0});
    // this->rotation = glm::rotate(glm::mat4(1.f), glm::radians(0.f), {0.f, 1.f, 0.f}); // no rotation to start
    this->model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
    // FIXME: maybe we alter the rotation of this, instead of the perspective being negative?
    updateMVP();
}

/// @brief Sets all the values in the depth buffer to zero
void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(this->height, std::vector<float>(this->width, 0.0));
}

/// @brief Translates camera position
void Camera::translate(Axis axis, int sign) {
    float modifier = 0.03f * sign;
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
    updateMVP();
}

/// @brief Rotates camera position anti-clockwise about the world origin
void Camera::rotate(Axis axis, int sign) {
    float modifier = 3 * sign;
    glm::vec3 axisVector;
    switch(axis) {
        case x:
            axisVector = {1.0f, 0.0f, 0.0f};
            break;
        case y:
            axisVector = {0.0f, 1.0f, 0.0f};
            break;
        default:
            return;
    }
    this->rotation = glm::rotate(this->rotation, glm::radians(modifier), axisVector);
    updateMVP();
}

void Camera::updateMVP() {
    glm::mat4 T0 = glm::translate(glm::mat4(1.f), this->position);
    glm::mat4 cameraMatrix = this->rotation * T0; // M
    glm::mat4 view = glm::inverse(cameraMatrix); // V
    this->mvp = this->projection * view * this->model;
}

/// @brief Alters the camera orientation to face a given vertex
void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 forward = glm::normalize(this->position - vertex);
    glm::vec3 direction(0.0, 1.0, 0.0);
    glm::vec3 right = glm::cross(direction, forward);
    glm::vec3 up = glm::cross(forward, right);
    this->rotation = {glm::vec4(right, 0.f), glm::vec4(up, 0.f), glm::vec4(forward, 0.f), glm::vec4(0.f, 0.f, 0.f, 1.f)};
    updateMVP();
}