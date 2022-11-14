#include "3D.h"
#include <cmath>

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

std::vector<std::vector<float>> emptyDepthBuffer() {
    return std::vector<std::vector<float>>(HEIGHT, std::vector<float>(WIDTH, 0.0));
}

void rotateCameraPosition(glm::mat4 &camera, std::string axis, int sign) {
    // rotate the camera position about the centre OF THE SCENE!
    double modifier = 0.02 * sign;
    glm::mat4 transformationMatrix;
    if (axis == "x") {
        transformationMatrix = glm::mat4(
                1.0, 0.0, 0.0, 0.0, // first column
                0.0, cos(modifier), sin(modifier), 0.0, // second column
                0.0, -sin(modifier), cos(modifier), 0.0,  // third column
                0.0, 0.0, 0.0, 1.0
        );
    } else if (axis == "y") {
        transformationMatrix = glm::mat4(
                cos(modifier), 0.0, -sin(modifier), 0.0, // first column
                0.0, 1.0, 0.0, 0.0, // second column
                sin(modifier), 0.0, cos(modifier), 0.0, // third column
                0.0, 0.0, 0.0, 1.0
        );
    } else {
        return;
    }
    // we only apply this to cameraposition
    camera = transformationMatrix * camera;
}

void translateCameraPosition(glm::mat4 &camera, std::string axis, int sign) {
    glm::vec3 translationVector(0.0, 0.0, 0.0);
    float modifier = 0.02 * sign;
    if (axis == "x") {
        translationVector.x = modifier;
    } else if (axis == "y") {
        translationVector.y = modifier;
    } else if (axis == "z") {
        translationVector.z = modifier;
    }
    glm::mat4 transformationMatrix(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        translationVector.x, translationVector.y, translationVector.z, 1.0
    );
    camera = transformationMatrix * camera;
}

//glm::mat4 updateWorldToCamera(glm::vec3 &cameraPosition) {
//    glm::mat4 cameraToWorld = glm::mat4(
//        -1.0, 0.0, 0.0, cameraPosition.x,
//        0.0, 1.0, 0.0, cameraPosition.y,
//        0.0, 0.0, -1.0, cameraPosition.z,
//        0.0, 0.0, 0.0, 1.0
//    );
//    glm::mat4 worldToCamera = glm::inverse(cameraToWorld);
//    return worldToCamera;
//}

glm::vec3 transposePoint(glm::mat4 &camera, glm::vec3 vertexPosition) {
    glm::mat4 worldToCamera = glm::inverse(camera);
    glm::vec4 newVertexPosition(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0);
    glm::vec4 cameraPoint = worldToCamera * newVertexPosition;
    return {cameraPoint.x, cameraPoint.y, cameraPoint.z};
}

CanvasPoint getCanvasIntersectionPoint(glm::mat4 &camera, glm::vec3 vertexPosition, float focalLength) {
    // note +z is towards you, -z is away from you
    glm::vec3 transposedVertexPosition = transposePoint(camera, vertexPosition);
    float u = focalLength * (WIDTH/2) * (transposedVertexPosition.x/(-1*transposedVertexPosition.z)) + WIDTH/2;
    float v = focalLength * (HEIGHT/2) * (transposedVertexPosition.y/(-1*transposedVertexPosition.z)) + HEIGHT/2;
    //std::cout << transposedVertexPosition.z << std::endl;
    return CanvasPoint(u, v, transposedVertexPosition.z);
}