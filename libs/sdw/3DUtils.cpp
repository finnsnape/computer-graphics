#include "3DUtils.h"

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

std::vector<std::vector<float>> emptyDepthBuffer() {
    return std::vector<std::vector<float>>(HEIGHT, std::vector<float>(WIDTH, 0.0));
}

glm::vec3 transposePoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition) {
    return {-1*(cameraPosition.x-vertexPosition.x), cameraPosition.y-vertexPosition.y, cameraPosition.z-vertexPosition.z};
}

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength) {
    // note +z is towards you, -z is away from you
    glm::vec3 transposedVertexPosition = transposePoint(cameraPosition, vertexPosition);
    float u = focalLength * (HEIGHT/2) * (transposedVertexPosition.x/transposedVertexPosition.z) + WIDTH/2;
    float v = focalLength * (HEIGHT/2) * (transposedVertexPosition.y/transposedVertexPosition.z) + HEIGHT/2;
    //std::cout << transposedVertexPosition.z << std::endl;
    return CanvasPoint(u, v, transposedVertexPosition.z);
}