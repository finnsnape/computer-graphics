#include "Camera.h"
#include <cmath>
#include <CanvasPoint.h>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(float _width, float _height, glm::vec3 _position, bool _orbit): width(_width), height(_height), position(_position), orbit(_orbit) {
    this->resetDepthBuffer();
    //this->lookAt({0.0, 0.0, 0.0});
    this->rotate(y, 1);
    this->projection = glm::perspective(glm::radians(-45.0f), -width/height, 0.1f, 100.0f);
    this->near = 0.1f;
    this->far = 100.f;
    this->model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
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

/// @brief Rotates camera position anti-clockwise about the camera coordinate system origin
void Camera::rotate(Axis axis, int sign) {
    float modifier = 3 * sign;
    glm::vec3 axisVector;
    // FIXME: a rotation of 0 is _slightly_ rotated because of floating point issues
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
    //this->orientation = {right, up, forward};
}