#include "Camera.h"

#include <CanvasPoint.h>


Camera::Camera(float _width, float _height, glm::vec3 _position, bool _orbit): width(_width), height(_height), position(_position), orbit(_orbit) {
    //this->lookAt({0.0, 0.0, 0.0});
    this->rotation = glm::rotate(glm::mat4(1.f), glm::radians(0.f), {0.f, 1.f, 0.f}); // no rotation to start
    this->model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
    // FIXME: maybe we alter the rotation of this, instead of the perspective being negative?
    this->updateMVP();
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
    this->updateMVP();
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
    this->updateMVP();
}

void Camera::updateMVP() {
    glm::mat4 T0 = glm::translate(glm::mat4(1.f), this->position);
    glm::mat4 cameraMatrix = this->rotation * T0; // M
    glm::mat4 view = glm::inverse(cameraMatrix); // V
    this->mvp = this->projection * view * this->model;
}

void pm(glm::mat4 m) {
    std::cout <<
    m[0].x << "," << m[0].y << "," << m[0].z << "," << m[0].w << std::endl <<
    m[1].x << "," << m[1].y << "," << m[1].z << "," << m[1].w << std::endl <<
    m[2].x << "," << m[2].y << "," << m[2].z << "," << m[2].w << std::endl <<
    m[3].x << "," << m[3].y << "," << m[3].z << "," << m[3].w << std::endl <<
    std::endl;
}

/// @brief Alters the camera orientation to face a given vertex
void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 forward = glm::normalize(vertex - this->position);
    glm::vec3 up(0.f, 1.f, 0.f);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    up = glm::cross(right, forward);
    glm::mat4 view(
        right.x, up.x, -forward.x, 1.f,
        right.y, up.y, -forward.y, 1.f,
        right.z, up.z, -forward.z, 1.f,
        -glm::dot(right, this->position), -glm::dot(up, this->position), glm::dot(forward, this->position), 1.f
    );
    // FIXME: we need to either update rotation or position as well
    //glm::mat4 T0 = glm::translate(glm::mat4(1.f), this->position);
    //this->rotation = glm::inverse(view) * glm::inverse(T0);
    //this->position = glm::vec3(-view[3]);
    this->mvp = this->projection * view * this->model;
}