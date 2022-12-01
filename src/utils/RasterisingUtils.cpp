#include "RasterisingUtils.h"
#include "Camera.h"
#include "Scene.h"
#include <ModelTriangle.h>
#include <CanvasTriangle.h>
#include <CanvasPoint.h>
#include <glm/glm.hpp>
#include "TriangleUtils.h"
#include <glm/gtc/matrix_transform.hpp>

namespace {
    CanvasPoint modelToScreen(Camera &camera, glm::vec3 vertex) {
        float near = 0.1f;
        float width = 240.0f;
        float height = 240.0f;
        float far = 100.0f;
        auto rotationDelta = glm::radians(0.0f);
        glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // y axis
        glm::vec3 cameraPosition(0.0f, 0.0f, 4.0f);
        glm::mat4 identity(1.0f);
        glm::mat4 T0 = glm::translate(identity, cameraPosition);
        glm::mat4 Ry = glm::rotate(identity, rotationDelta, rotationAxis);
        glm::mat4 cameraMatrix = Ry * T0; // M
        glm::mat4 viewMatrix = glm::inverse(cameraMatrix); // V

        glm::mat4 worldMatrix = glm::translate(identity, {0.0f, 0.0f, 0.0f}); // takes position of cube (we can place cube in centre of scene?)

        glm::mat4 projMatrix = glm::perspective(glm::radians(-45.0f), -width/height, 0.1f, 100.0f);

        glm::vec4 clipPos = projMatrix * viewMatrix * worldMatrix * glm::vec4(vertex, 1.0);

        glm::vec3 ndsPos(clipPos.x/clipPos.w, clipPos.y/clipPos.w, clipPos.z/clipPos.w); // normalised device space

        CanvasPoint screenPos(width/2 * (ndsPos.x + 1), height/2 * (ndsPos.y + 1), ndsPos.z * (far - near)/2 + (far + near)/2);
        return screenPos;
    }

    glm::vec3 transposePoint(Camera &camera, glm::vec3 vertex) {
        glm::vec3 vertexDifference = camera.position - vertex;
        glm::vec3 transposedPoint = camera.orientation * vertexDifference;
        return {transposedPoint.x, transposedPoint.y, transposedPoint.z};
    }

    glm::vec3 transposePoint2(Camera &camera, glm::vec3 vertex) {
        glm::mat4 view(
                camera.orientation[0][0], camera.orientation[0][1], camera.orientation[0][2], camera.position.x,
                camera.orientation[1][0], camera.orientation[1][1], camera.orientation[1][2], camera.position.y,
                camera.orientation[2][0], camera.orientation[2][1], camera.orientation[2][2], camera.position.z,
                0.0, 0.0, 0.0, 1.0
        );
    }

    CanvasPoint calculateCanvasIntersection(Scene &scene, glm::vec3 vertex) {
        glm::vec3 transposedVertex = transposePoint(scene.camera, vertex);
        float u = -scene.camera.focalLength * (scene.width/2) * (transposedVertex.x/transposedVertex.z) + scene.width/2;
        float v = scene.camera.focalLength * (scene.height/2) * (transposedVertex.y/transposedVertex.z) + scene.height/2;
        return {u, v, transposedVertex.z};
    }

    CanvasTriangle makeCanvasTriangle(Scene &scene, ModelTriangle triangle) {
        CanvasTriangle canvasTriangle;
        for (int i=0; i<3; i++) {
            glm::vec3 vertex = triangle.vertices[i];
            CanvasPoint canvasIntersection = modelToScreen(scene.camera, vertex);
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

    void drawStroked(Camera &camera) {
        glm::vec3 t =  transposePoint(camera, {0.0, 0.0, 0.0});
        std::cout << t.x << "," << t.y << "," << t.z << std::endl;
//        for (const auto &triangle : scene.triangles) {
//            CanvasTriangle canvasTriangle = makeCanvasTriangle(scene, triangle);
//            TriangleUtils::drawStrokedTriangle(scene, canvasTriangle);
//        }
    }
}

