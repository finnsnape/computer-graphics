#include "RayTracingUtils.h"

namespace {
    glm::vec3 calculateRawIntersection(Camera &camera, ModelTriangle triangle, glm::mat3 DEMatrix) {
        glm::vec3 SPVector = camera.position - triangle.vertices[0];
        glm::vec3 rawIntersection = glm::inverse(DEMatrix) * SPVector;
        return rawIntersection;
    }

    bool validateRawIntersection(glm::vec3 rawIntersection) {
        float t = rawIntersection.x;
        float u = rawIntersection.y;
        float v = rawIntersection.z;
        if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && u + v <= 1.0 && t > 0) {
            return true;
        }
        return false;
    }

    glm::vec3 calculateIntersection(ModelTriangle triangle, glm::mat3 DEMatrix, glm::vec3 rawIntersection) {
        glm::vec3 e0 = DEMatrix[1];
        glm::vec3 e1 = DEMatrix[2];
        glm::vec3 intersectionPoint = triangle.vertices[0] + rawIntersection.y * e0 + rawIntersection.z * e1;
        return intersectionPoint;
    }

    glm::mat3 calculateDEMatrix(ModelTriangle triangle, glm::vec3 rayDirection) {
        glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
        glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
        glm::mat3 DEMatrix(-rayDirection, e0, e1);
        return DEMatrix;
    }

    /// @brief Returns the normalised direction vector from the camera position to the point on the canvas, in 3D space
    glm::vec3 calculateRayDirection(Scene &scene, CanvasPoint canvasPoint) {
        float z = 2.0;
        float x = z * (canvasPoint.x - scene.width/2)/(scene.width/2 * scene.camera.focalLength);
        auto y = z * (canvasPoint.y - scene.height/2)/(scene.height/2 * -scene.camera.focalLength);
        return glm::normalize(glm::vec3(x, y, z) - scene.camera.position);
    }
}

namespace RayTracingUtils {
    /// @brief Returns the triangle that the ray corresponding to the given canvas point first intersects with
    RayTriangleIntersection findClosestTriangle(Scene &scene, CanvasPoint canvasPoint) {
        glm::vec3 rayDirection = calculateRayDirection(scene, canvasPoint);
        RayTriangleIntersection closestTriangle;
        closestTriangle.distanceFromCamera = FLT_MAX;
        for (size_t i=0; i<scene.triangles.size(); i++) {
            ModelTriangle triangle = scene.triangles[i];
            glm::mat3 DEMatrix = calculateDEMatrix(triangle, rayDirection);
            if (glm::determinant(DEMatrix) == 0) continue; // would be dividing by 0
            glm::vec3 rawIntersection = calculateRawIntersection(scene.camera, triangle, DEMatrix);
            if (!validateRawIntersection(rawIntersection)) continue;
            if (rawIntersection.x >= closestTriangle.distanceFromCamera) continue; // something closer already exists
            glm::vec3 intersectionPoint = calculateIntersection(triangle, DEMatrix, rawIntersection);
            closestTriangle = {intersectionPoint, rawIntersection.x, triangle, i};
        }
        return closestTriangle;
    }
}