#pragma once

#include <glm/glm.hpp>
#include <vector>

class Camera {
private:
    float width;
    float height;
    //glm::vec4 position;
public:
    void updateView();
    void printMatrix(glm::mat4 matrix);
    enum Axis { x, y, z };
    float focalLength;
    glm::mat4 view;
    glm::mat4 camera;
    glm::mat4 projection;
    glm::vec3 position;
    glm::mat3 orientation;
    bool orbit;
    std::vector<std::vector<float>> depthBuffer;
    Camera(float width, float height, float focalLength, glm::vec3 position, bool orbit);
    void resetDepthBuffer();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
};
