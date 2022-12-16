#include "Camera.h"

#include <CanvasPoint.h>


Camera::Camera(float _width, float _height, glm::vec3 _position, bool _orbit): width(_width), height(_height), position(_position), orbit(_orbit) {
    this->model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f}); // place object at origin
    this->lookAt({0.0, 0.0, 0.0});
}

/// @brief Sets all the values in the depth buffer to zero
void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(this->height, std::vector<float>(this->width, 0.f));
}

/// @brief Translates camera position
void Camera::translate(Axis axis, float sign) {
    float delta = 0.03f * sign;
    glm::mat4 translationMatrix(1.f);
    glm::vec4 translationVector(0.f, 0.f, 0.f, 1.f);
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
    translationMatrix[3] = translationVector;
    this->camera = translationMatrix * this->camera;
    this->updateMVP();
}

/// @brief Rotates camera position anti-clockwise about the world origin
void Camera::rotate(Axis axis, float sign) {
    float delta = glm::radians(3.f * sign);
    glm::mat4 rotationMatrix;
    float cos = std::cos(delta);
    float sin = std::sin(delta);
    switch(axis) {
        case x:
            rotationMatrix = glm::mat4(
                1.f, 0.f, 0.f, 0.f,
                0.f, cos, -sin, 0.f,
                0.f, sin, cos, 0.f,
                0.f, 0.f, 0.f, 1.f
            );
            break;
        case y:
            rotationMatrix = glm::mat4(
                cos, 0.f, sin, 0.f,
                0.f, 1.f, 0.f, 0.f,
                -sin, 0.f, cos, 0.f,
                0.f, 0.f, 0.f, 1.f
            );
            break;
        default:
            return;
    }
    this->camera = rotationMatrix * this->camera;
    this->updateMVP();
}

/// @brief Updates the MVP matrix after a change to the camera matrix
void Camera::updateMVP() {
    glm::mat4 view = glm::inverse(this->camera);
    this->position = glm::vec3(this->camera[3]); // get first 3 coords from 4th column
    this->mvp = this->projection * view * this->model;
}

/// @brief Alters the camera orientation to face a given vertex
void Camera::lookAt(glm::vec3 vertex) {
    // FIXME: if we rotate upside down and then lookAt, everything is swapped
    // i.e., left is now right, right is now left
    // may relate to our negatives in perspective function
    glm::vec3 forward = glm::normalize(vertex - this->position);
    glm::vec3 up(0.f, 1.f, 0.f);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    up = glm::cross(right, forward);
    this->camera = glm::inverse(glm::mat4(
            right.x, up.x, -forward.x, 0.f,
            right.y, up.y, -forward.y, 0.f,
            right.z, up.z, -forward.z, 0.f,
            -glm::dot(right, this->position), -glm::dot(up, this->position), glm::dot(forward, this->position), 1.f
    ));
    this->updateMVP();
}
