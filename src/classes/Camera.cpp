#include "Camera.h"

#include <CanvasPoint.h>


Camera::Camera(float _width, float _height, glm::vec3 _position, bool _orbit): width(_width), height(_height), position(_position), orbit(_orbit) {
    this->model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
    this->lookAt({0.0, 0.0, 0.0});
}

/// @brief Sets all the values in the depth buffer to zero
void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(this->height, std::vector<float>(this->width, 0.f));
}

/// @brief Translates camera position
void Camera::translate(Axis axis, float sign) {
    float delta = 0.03f * sign;
    glm::vec3 translationVector(0.f, 0.f, 0.f);
    switch(axis) {
        case x:
            translationVector.x = delta;
            break;
        case y:
            translationVector.y = delta;
            break;
        case z:
            translationVector.z = delta;
            break;
        default:
            break;
    }
    // FIXME: figure out ourselves
    this->view = glm::translate(this->view, -translationVector);
    this->updateMVP();
}

/// @brief Rotates camera position anti-clockwise about the world origin
void Camera::rotate(Axis axis, float sign) {
    float delta = 3.f * sign;
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
    // FIXME: figure out ourselves
    this->view = glm::rotate(this->view, glm::radians(delta), axisVector);
    this->updateMVP();
}

/// @brief Updates the MVP matrix after a change to the view matrix
void Camera::updateMVP() {
    glm::mat4 camera = glm::inverse(this->view);
    this->position = glm::vec3(camera[3]); // get first 3 coords from 4th column
    this->mvp = this->projection * this->view * this->model;
}

/// @brief Alters the camera orientation to face a given vertex
void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 up(0.f, 1.f, 0.f);
    // FIXME: figure out ourselves
    this->view = glm::lookAt(this->position, vertex, up);
    this->updateMVP();
}