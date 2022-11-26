#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(float _width, float _height, float _focalLength, glm::vec3 _position): width(_width), height(_height), focalLength(_focalLength), position(_position) {
    this->resetDepthBuffer();
    this->lookAt({0.0, 0.0, 0.0});
}

/// @brief Sets all the values in the depth buffer to zero
void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(this->height, std::vector<float>(this->width, 0.0));
}

/// @brief Translates camera position
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

/// @brief Rotates camera position anti-clockwise about the camera coordinate system origin
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
    this->position = transformationMatrix * this->position;
}

/// @brief Alters the camera orientation to face a given vertex
void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 forward = glm::normalize(this->position - vertex);
    glm::vec3 direction(0.0, 1.0, 0.0);
    glm::vec3 right = glm::cross(direction, forward);
    glm::vec3 up = glm::cross(forward, right);
    this->orientation = {right, up, forward};
}

/// @brief Rotates the camera position and then adjusts orientation to look at the given vertex
void Camera::orbit(Axis axis, int sign, glm::vec3 vertex) {
    // doesn't work for x axis, need to change direction vector?
    this->rotate(axis, sign);
    this->lookAt(vertex);
}

//void printMatrix(glm::mat3 matrix) {
//    std::cout << "Matrix: " <<
//    matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << std::endl <<
//    matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << std::endl <<
//    matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << std::endl <<
//    std::endl;
//}
