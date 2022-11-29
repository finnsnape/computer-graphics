#include "RasterisingUtils.h"
#include "Camera.h"
#include "Scene.h"
#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <CanvasPoint.h>
#include <glm/glm.hpp>
#include "TriangleUtils.h"

namespace {
    glm::vec3 transposePoint(Camera &camera, glm::vec3 vertex) {
        glm::vec3 vertexDifference = vertex - camera.position;
        glm::vec3 transposedPoint = vertexDifference * camera.orientation;
        return {transposedPoint.x, transposedPoint.y, -transposedPoint.z};
    }

    CanvasPoint calculateCanvasIntersection(Scene &scene, glm::vec3 vertex) {
        glm::vec3 transposedVertex = transposePoint(scene.camera, vertex);
        float u = scene.camera.focalLength * (scene.width/2) * (transposedVertex.x/transposedVertex.z) + scene.width/2;
        float v = -scene.camera.focalLength * (scene.height/2) * (transposedVertex.y/transposedVertex.z) + scene.height/2;
        return {u, v, transposedVertex.z};
    }

    CanvasTriangle makeCanvasTriangle(Scene &scene, ModelTriangle triangle) {
        CanvasTriangle canvasTriangle;
        for (int i=0; i<3; i++) {
            glm::vec3 vertex = triangle.vertices[i];
            CanvasPoint canvasIntersection = calculateCanvasIntersection(scene, vertex);
            canvasTriangle.vertices[i] = canvasIntersection;
        }
        return canvasTriangle;
    }
}

namespace RasterisingUtils {
    void drawFilled(Scene &scene) {
        scene.camera.resetDepthBuffer();
        for (const auto &triangle : scene.triangles) {
            CanvasTriangle canvasTriangle = makeCanvasTriangle(scene, triangle);
            TriangleUtils::drawFilledTriangle(scene, canvasTriangle, triangle.colour);
        }
    }

    void drawStroked(Scene &scene) {
        for (const auto &triangle : scene.triangles) {
            CanvasTriangle canvasTriangle = makeCanvasTriangle(scene, triangle);
            TriangleUtils::drawStrokedTriangle(scene, canvasTriangle);
        }
    }
}

