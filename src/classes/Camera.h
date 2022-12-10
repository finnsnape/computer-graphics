#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    float width;
    float height;
    glm::mat4 rotation;
    glm::mat4 model;
    const glm::mat4 projection = glm::perspective(glm::radians(-45.f), -width/height, 0.1f, 100.f);
    void updateMVP();
public:
    enum Axis { x, y, z };
    glm::mat4 mvp;
    glm::vec3 position;
    const float near = 0.1f;
    const float far = 100.f;
    bool orbit;
    std::vector<std::vector<float>> depthBuffer;
    Camera(float width, float height, glm::vec3 position, bool orbit);
    void resetDepthBuffer();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
};
