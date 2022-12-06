#pragma once

#include <glm/glm.hpp>
#include <vector>

class Camera {
private:
    float width;
    float height;
    glm::vec3 position;
    glm::mat4 rotation;
    glm::mat4 model;
    glm::mat4 projection;
    void updateMVP();
public:
    enum Axis { x, y, z };
    glm::mat4 mvp;
    float near;
    float far;
    bool orbit;
    std::vector<std::vector<float>> depthBuffer;
    Camera(float width, float height, glm::vec3 position, bool orbit);
    void resetDepthBuffer();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
};
