#include "3DUtils.h"
#include <cmath>

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

std::vector<std::vector<float>> emptyDepthBuffer() {
    return std::vector<std::vector<float>>(HEIGHT, std::vector<float>(WIDTH, 0.0));
}

void rotatePosition(glm::vec3 &cameraPosition, std::string axis, int sign) {
    double modifier = 2.0;
    glm::mat3 matrix;
    if (axis == "x") {
        matrix = glm::mat3(
            1.0, 0.0, 0.0, // first column
            0.0, cos(modifier), sin(modifier), // second column
            0.0, -sin(modifier), cos(modifier)  // third column
        );
    } else if (axis == "y") {
        matrix = glm::mat3(
            cos(modifier), 0.0, -sin(modifier), // first column
            0.0, 1.0, 0.0, // second column
            sin(modifier), 0.0, cos(modifier)  // third column
        );
    } else {
        return;
    }
    cameraPosition = glm::vec3(matrix * cameraPosition);
    std::cout << cameraPosition.x << "," << cameraPosition.y << "," << cameraPosition.z << std::endl;
}

void translatePosition(glm::vec3 &cameraPosition, std::string axis, int sign) {
    float modifier = 0.02 * sign;
    if (axis == "x") {
        cameraPosition.x = cameraPosition.x + modifier;
    } else if (axis == "y") {
        cameraPosition.y = cameraPosition.y + modifier;
    } else if (axis == "z") {
        cameraPosition.z = cameraPosition.z + modifier;
    }
}

glm::vec3 transposePoint(glm::vec3 &cameraPosition, glm::vec3 vertexPosition) {
    return {-1*(cameraPosition.x-vertexPosition.x), cameraPosition.y-vertexPosition.y, cameraPosition.z-vertexPosition.z};
}

CanvasPoint getCanvasIntersectionPoint(glm::vec3 &cameraPosition, glm::vec3 vertexPosition, float focalLength) {
    // note +z is towards you, -z is away from you
    glm::vec3 transposedVertexPosition = transposePoint(cameraPosition, vertexPosition);
    float u = focalLength * (HEIGHT/2) * (transposedVertexPosition.x/transposedVertexPosition.z) + WIDTH/2;
    float v = focalLength * (HEIGHT/2) * (transposedVertexPosition.y/transposedVertexPosition.z) + HEIGHT/2;
    //std::cout << transposedVertexPosition.z << std::endl;
    return CanvasPoint(u, v, transposedVertexPosition.z);
}