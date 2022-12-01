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
    CanvasPoint modelToScreen(Scene &scene, glm::vec3 vertex) {
        float near = 0.1f;
        float far = 100.0f;
        glm::mat4 identity(1.0f);
        glm::mat4 T0 = glm::translate(identity, scene.camera.position);
        glm::mat4 cameraMatrix = scene.camera.rotation * T0; // M
        glm::mat4 viewMatrix = glm::inverse(cameraMatrix); // V

        glm::vec4 clipPos = scene.camera.projection * viewMatrix * scene.camera.model * glm::vec4(vertex, 1.0);

        glm::vec3 ndsPos(clipPos.x/clipPos.w, clipPos.y/clipPos.w, clipPos.z/clipPos.w); // normalised device space

        CanvasPoint screenPos(scene.width/2 * (ndsPos.x + 1), scene.height/2 * (ndsPos.y + 1), ndsPos.z * (far - near)/2 + (far + near)/2);
        return screenPos;
    }

    CanvasTriangle makeCanvasTriangle(Scene &scene, ModelTriangle triangle) {
        CanvasTriangle canvasTriangle;
        for (int i=0; i<3; i++) {
            glm::vec3 vertex = triangle.vertices[i];
            CanvasPoint canvasIntersection = modelToScreen(scene, vertex);
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

