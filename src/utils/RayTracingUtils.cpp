#include "RayTracingUtils.h"
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <RayTriangleIntersection.h>
#include "Scene.h"
#include "TriangleUtils.h"

namespace {
    /// @brief Gets the absolute distance along ray (t), and proportional distances along triangle edges (u, v)
    glm::vec3 calculateRawIntersection(glm::vec3 from, ModelTriangle triangle, glm::mat3 DEMatrix) {
        glm::vec3 SPVector = from - triangle.vertices[0];
        glm::vec3 rawIntersection = glm::inverse(DEMatrix) * SPVector;
        return rawIntersection;
    }

    /// @brief Ensures the distances are on the triangle edges, and the distance along the ray is positive
    bool validateRawIntersection(glm::vec3 rawIntersection) {
        float t = rawIntersection.x;
        float u = rawIntersection.y;
        float v = rawIntersection.z;
        if (u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && u + v <= 1.0 && t > 0) {
            return true;
        }
        return false;
    }

    /// @brief Finds the relevant 3D point on the intersecting triangle
    glm::vec3 calculateIntersection(Camera &camera, glm::mat3 DEMatrix, ModelTriangle triangle, glm::vec3 rawIntersection) {
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
        // convert canvas point to 3D point in camera coordinate space
        float z = scene.camera.position.z - scene.camera.focalLength;
        float x = z * (canvasPoint.x - scene.width/2)/(scene.width/2 * scene.camera.focalLength);
        float y = z * (canvasPoint.y - scene.height/2)/(scene.height/2 * -scene.camera.focalLength);
        // then ray is normalised difference:
        // note we should do orientation * what we return!

        return glm::normalize(scene.camera.orientation * (glm::vec3(x, y, z) - scene.camera.position));
    }

    /// @brief Returns the triangle that the given ray intersects with first, using position "from" (camera or light source)
    RayTriangleIntersection findClosestTriangle(Scene &scene, glm::vec3 rayDirection, glm::vec3 from) {
        RayTriangleIntersection closestTriangle;
        closestTriangle.distanceFromCamera = FLT_MAX;
        for (size_t i=0; i<scene.triangles.size(); i++) {
            ModelTriangle triangle = scene.triangles[i];
            glm::mat3 DEMatrix = calculateDEMatrix(triangle, rayDirection);
            if (glm::determinant(DEMatrix) == 0) continue; // would be dividing by 0
            glm::vec3 rawIntersection = calculateRawIntersection(from, triangle, DEMatrix);
            if (!validateRawIntersection(rawIntersection)) continue;
            if (rawIntersection.x >= closestTriangle.distanceFromCamera) continue; // something closer already exists
            glm::vec3 intersectionPoint = calculateIntersection(scene.camera, DEMatrix, triangle, rawIntersection);
            closestTriangle = {intersectionPoint, rawIntersection.x, triangle, i};
        }
        return closestTriangle;
    }

    /// @brief Checks if the point we want to draw on an intersecting triangle is able to see the light source
    bool canSeeLight(Scene &scene, const RayTriangleIntersection &closestTriangle) {
        // ray from intersection to light source
        glm::vec3 rayDirection = glm::normalize(closestTriangle.intersectionPoint - scene.lightSource);
        // get triangle closest to light source
        RayTriangleIntersection newClosestTriangle = findClosestTriangle(scene, rayDirection, scene.lightSource);
        // if triangle we are trying to draw is the triangle closest to the light source
        // then that means it can see the light. otherwise, it cannot as it's behind another one
        if (closestTriangle.triangleIndex == newClosestTriangle.triangleIndex) {
            return true;
        }
        return false;
    }
}

namespace RayTracingUtils {
    void draw(Scene &scene, bool shadows) {
        for (int x=0; x<scene.width; x++) {
            for (int y=0; y<scene.height; y++) {
                CanvasPoint canvasPoint((float) x, (float) y);
                if (!TriangleUtils::isInsideCanvas(scene.window, canvasPoint)) continue;
                glm::vec3 rayDirection = calculateRayDirection(scene, canvasPoint);
                //rayDirection.z = -rayDirection.z;
                RayTriangleIntersection closestTriangle = findClosestTriangle(scene, rayDirection, scene.camera.position);
                if (closestTriangle.distanceFromCamera == FLT_MAX) {
                    continue; // no triangle intersection found
                }
                Colour pixelColour = closestTriangle.intersectedTriangle.colour;
                if (shadows && !canSeeLight(scene, closestTriangle)) {
                    pixelColour = {0, 0, 0}; // in shadow
                }
                // FIXME: need to handle drawing pixels outside window
                TriangleUtils::drawPixel(scene.window, canvasPoint, pixelColour);
            }
        }
    }
}