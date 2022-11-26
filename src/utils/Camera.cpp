#include "Camera.h"
#include <cmath>
#include <signal.h>
#include <memory>
#include <iostream>

#define WIDTH 240
#define HEIGHT 240


Camera::Camera(glm::vec3 _position, float _focalLength): focalLength(_focalLength), position(_position) {
    this->resetDepthBuffer();
    this->lookAt({0.0, 0.0, 0.0});
}

void Camera::resetDepthBuffer() {
    this->depthBuffer = std::vector<std::vector<float>>(HEIGHT, std::vector<float>(WIDTH, 0.0));
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

/// @brief Transposes a point from the model coordinate system to the camera coordinate system
glm::vec3 Camera::getTransposedPoint(glm::vec3 vertex) {
    glm::vec3 vertexDifference = vertex - this->position;
    glm::vec3 transposedVertex = vertexDifference * this->orientation;
    return {transposedVertex.x, transposedVertex.y, -transposedVertex.z};
}

/// @brief Finds where a vertex in the model coordinate system should be drawn on the 2D canvas
CanvasPoint Camera::getIntersectionPoint(glm::vec3 vertex) {
    glm::vec3 transposedVertex = this->getTransposedPoint(vertex);
    float u = -this->focalLength * (WIDTH/2) * (transposedVertex.x/(transposedVertex.z)) + WIDTH/2;
    float v = this->focalLength * (HEIGHT/2) * (transposedVertex.y/(transposedVertex.z)) + HEIGHT/2;
    return {u, v, transposedVertex.z};
}

/// @brief Calculates the direction vector for a given ray originating from the camera position
glm::vec3 Camera::getRayDirection(CanvasPoint canvasPoint) {
    float z = 2.0;
    auto x = z * (canvasPoint.x - WIDTH/2)/(WIDTH/2 * this->focalLength);
    auto y = z * (canvasPoint.y - HEIGHT/2)/(HEIGHT/2 * -this->focalLength);
    glm::vec3 canvasPoint3D(x, y, z);
    return glm::normalize(canvasPoint3D - this->position);
}

void printMatrix(glm::mat3 matrix) {
    std::cout << "Matrix: " <<
    matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << std::endl <<
    matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << std::endl <<
    matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << std::endl <<
    std::endl;
}

glm::mat3 Camera::getDEMatrix(ModelTriangle triangle, glm::vec3 rayDirection) {
    glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
    glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
    glm::mat3 DEMatrix(-rayDirection, e0, e1);
    return DEMatrix;
}

/// @brief Calculates a possible ray solution
glm::vec3 Camera::getRaySolution(ModelTriangle triangle, glm::mat3 DEMatrix) {
    glm::vec3 SPVector = this->position - triangle.vertices[0];
    glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
    return possibleSolution;
}

/// @brief Checks potential solution is valid
bool Camera::validateRaySolution(glm::vec3 possibleSolution) {
    float t = possibleSolution.x;
    float u = possibleSolution.y;
    float v = possibleSolution.z;

    if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && u + v <= 1.0 && t > 0) {
        return true;
    }
    return false;
}

/// @brief Calculates where a ray intersects with the given triangle
glm::vec3 Camera::getRayIntersection(ModelTriangle triangle, glm::vec3 possibleSolution) {
    glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
    glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
    glm::vec3 intersectionPoint = triangle.vertices[0] + possibleSolution.y * e0 + possibleSolution.z * e1;
    return intersectionPoint;
}

/// @brief Finds the triangle (if there is one) that the ray intersects with first
RayTriangleIntersection Camera::getClosestIntersection(std::vector<ModelTriangle> triangles, glm::vec3 rayDirection) {
    RayTriangleIntersection solution;
    solution.exists = false;
    solution.distanceFromCamera = FLT_MAX;
    for (size_t i=0; i<triangles.size(); i++) {
        ModelTriangle triangle = triangles[i];
        glm::mat3 DEMatrix = this->getDEMatrix(triangle, rayDirection);
        if (glm::determinant(DEMatrix) == 0) continue; // would be dividing by 0
        glm::vec3 possibleSolution = this->getRaySolution(triangle, DEMatrix);
        if (!this->validateRaySolution(possibleSolution)) continue;
        if (possibleSolution.x >= solution.distanceFromCamera) continue; // something closer already exists
        glm::vec3 intersectionPoint = this->getRayIntersection(triangle, possibleSolution);
        solution = {true, intersectionPoint, possibleSolution.x, triangle, i}; // TODO: don't need to do this every time
    }
    return solution;
}

/// @brief Rotates the camera position and then adjusts orientation to look at the given vertex
void Camera::orbit(Axis axis, int sign, glm::vec3 vertex) {
    // doesn't work for x axis, need to change direction vector?
    this->rotate(axis, sign);
    this->lookAt(vertex);
}
