#include "Camera.h"
#include <cmath>

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

Camera::Camera(glm::vec3 _position, float _focalLength): focalLength(_focalLength), position(_position) {
    this->resetDepthBuffer();
    this->lookAt({0.0, 0.0, 0.0});
}

void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(HEIGHT, std::vector<float>(WIDTH, 0.0));
}

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

void Camera::rotate(Axis axis, int sign) {
    // anti-clockwise
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

//void Camera::lookAt(glm::vec3 vertex) {
//    glm::vec3 forward = glm::normalize(this->position - vertex);
//    glm::vec3 direction(0.0, 1.0, 0.0);
//    glm::vec3 right = glm::cross(forward, direction);
//    glm::vec3 up = glm::cross(forward, right);
//    this->orientation = {right, up, forward};
//}

void Camera::lookAt(glm::vec3 vertex) {
    glm::vec3 forward = glm::normalize(this->position - vertex);
    glm::vec3 direction(0.0, 1.0, 0.0);
    glm::vec3 right = glm::cross(direction, forward);
    glm::vec3 up = glm::cross(forward, right);
    this->orientation = {right, up, forward};
}

//glm::vec3 Camera::getTransposedPoint(glm::vec3 vertex) const {
//    glm::vec3 vertexDifference = vertex - this->position;
//    return vertexDifference * this->orientation;
//}

void Camera::transposeTriangle(ModelTriangle &triangle) {
    for (auto &vertex : triangle.vertices) {
        vertex = this->getTransposedPoint(vertex);
    }
}

glm::vec3 Camera::getTransposedPoint(glm::vec3 vertex) const {
    glm::vec3 vertexDifference = vertex - this->position;
    glm::vec3 transposedVertex = vertexDifference * this->orientation;
    return {transposedVertex.x, transposedVertex.y, -transposedVertex.z};
}

//CanvasPoint Camera::getIntersectionPoint(glm::vec3 vertex) {
//    glm::vec3 transposedVertex = this->getTransposedPoint(vertex);
//    float u = this->focalLength * (WIDTH/2) * (transposedVertex.x/(transposedVertex.z)) + WIDTH/2;
//    float v = this->focalLength * (HEIGHT/2) * (transposedVertex.y/(transposedVertex.z)) + HEIGHT/2;
//    return {u, v, transposedVertex.z};
//}

CanvasPoint Camera::getIntersectionPoint(glm::vec3 vertex) {
    glm::vec3 transposedVertex = this->getTransposedPoint(vertex);
    float u = -this->focalLength * (WIDTH/2) * (transposedVertex.x/(transposedVertex.z)) + WIDTH/2;
    float v = this->focalLength * (HEIGHT/2) * (transposedVertex.y/(transposedVertex.z)) + HEIGHT/2;
    return {u, v, transposedVertex.z};
}

glm::vec3 Camera::getDepth(CanvasPoint canvasPoint) {
    float x = canvasPoint.x - WIDTH/2;
    float y = -canvasPoint.y + HEIGHT/2;
    float z = 0.0;
    glm::vec3 canvasPoint3D(x, y, z);
    return glm::normalize(canvasPoint3D - this->position);
}

//glm::vec3 Camera::getDirection(CanvasPoint canvasPoint) {
//    float z = this->getDepth(canvasPoint);
//    //float x = canvasPoint.x/(-this->focalLength * WIDTH/2);
//    //float y = canvasPoint.y/(this->focalLength * HEIGHT/2);
//    float x = (z * (canvasPoint.x - WIDTH/2))/(-this->focalLength * WIDTH/2);
//    float y = (z * (canvasPoint.y - HEIGHT/2))/(this->focalLength * HEIGHT/2);
//    glm::vec3 ray(x, y, z);
//    return glm::normalize(ray);
//}

bool Camera::validateIntersection(glm::vec3 possibleSolution) {
    float t = possibleSolution.x;
    float u = possibleSolution.y;
    float v = possibleSolution.z;

    if (t < 0.0) { // ensure point is in front of us
        return false;
    } else if (u < 0.0 || u > 1.0) { // ensure point is actually inside triangle (V)
        return false;
    } else if (v < 0.0 || v > 1.0) {
        return false;
    } else if (u + v > 1.0) {
        return false;
    }
    return true;
}

RayTriangleIntersection Camera::getClosestIntersection(std::vector<ModelTriangle> triangles, glm::vec3 rayDirection) {
    RayTriangleIntersection solution;
    solution.distanceFromCamera = INFINITY;
    for (size_t i=0; i<triangles.size(); i++) {
        ModelTriangle triangle = triangles[i];
        glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
        glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
        glm::vec3 SPVector = this->position - triangle.vertices[0];
        glm::mat3 DEMatrix(-rayDirection, e0, e1);
        glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
        if (possibleSolution.x > solution.distanceFromCamera) {
            continue;
        }
        if (!validateIntersection(possibleSolution)) {
            continue;
        }
        glm::vec3 intersectionPoint = possibleSolution.x * rayDirection;
        solution = {intersectionPoint, possibleSolution.x, triangle, i};
    }
    return solution;
}


void Camera::orbit(Axis axis, int sign, glm::vec3 vertex) {
    // doesn't work for x axis, need to change direction vector?
    this->rotate(axis, sign);
    this->lookAt(vertex);
}
