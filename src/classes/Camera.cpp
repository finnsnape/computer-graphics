#include "Camera.h"
#include <cmath>
#include <CanvasPoint.h>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(float _width, float _height, glm::vec3 _position, bool _orbit): width(_width), height(_height), position(_position), orbit(_orbit) {
    this->resetDepthBuffer();
    //this->lookAt({0.0, 0.0, 0.0});
    this->rotate(y, 1);
    this->projection = glm::perspective(glm::radians(-45.0f), -width/height, 0.1f, 100.0f);
    this->model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
}

/// @brief Sets all the values in the depth buffer to zero
void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(this->height, std::vector<float>(this->width, 0.0));
}

//void Camera::printMatrix(glm::mat4 matrix) {
//    std::cout <<
//    matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " " << matrix[3][0] << std::endl <<
//    matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " " << matrix[3][1] << std::endl <<
//    matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " " << matrix[3][2] << std::endl <<
//    matrix[0][3] << " " << matrix[1][3] << " " << matrix[2][3] << " " << matrix[3][3] << std::endl <<
//    std::endl;
//}

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
}

/// @brief Alters the camera orientation to face a given vertex
void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 forward = glm::normalize(this->position - vertex);
    glm::vec3 direction(0.0, 1.0, 0.0);
    glm::vec3 right = glm::cross(direction, forward);
    glm::vec3 up = glm::cross(forward, right);
    //this->orientation = {right, up, forward};
}