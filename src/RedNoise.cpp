#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <vector>
#include "utils/Triangles.h"
#include "utils/Files.h"
#include "utils/3D.h"
#include "utils/Camera.h"

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

void draw(DrawingWindow &window, Camera &camera, std::vector<ModelTriangle> modelTriangles) {
    window.clearPixels();
    //lookAt(camera, glm::vec3(0.0, 0.0, 0.0));
    std::vector<std::vector<float>> depthBuffer = emptyDepthBuffer();
    for (auto modelTriangle : modelTriangles) {
        std::vector<CanvasPoint> rawTriangle;
        for (int j = 0; j < 3; j++) {
            glm::vec3 vertex = modelTriangle.vertices[j];
            CanvasPoint intersectionPoint = camera.getIntersectionPoint(vertex);
            // std::cout << intersectionPoint << std::endl;
            rawTriangle.push_back(intersectionPoint);
        }
        CanvasTriangle triangle({rawTriangle[0], rawTriangle[1], rawTriangle[2]});
        drawFilledTriangle(window, depthBuffer, triangle, modelTriangle.colour);
    }
    camera.rotate(y, 1);
    camera.lookAt(glm::vec3(0.0, 0.0, 0.0));
}

void handleEvent(SDL_Event event, DrawingWindow &window, Camera camera) {
//	if (event.type == SDL_KEYDOWN) {
//		if (event.key.keysym.sym == SDLK_LEFT) {
//            translateCameraPosition(camera, "x", -1);
//        } else if (event.key.keysym.sym == SDLK_RIGHT) {
//            translateCameraPosition(camera, "x", 1);
//        } else if (event.key.keysym.sym == SDLK_UP) {
//            translateCameraPosition(camera, "y", 1);
//        } else if (event.key.keysym.sym == SDLK_DOWN) {
//            translateCameraPosition(camera, "y", -1);
//        } else if (event.key.keysym.sym == SDLK_MINUS) {
//            translateCameraPosition(camera, "z", 1);
//        } else if (event.key.keysym.sym == SDLK_EQUALS) {
//            translateCameraPosition(camera, "z", -1);
//        } else if (event.key.keysym.sym == SDLK_w) {
//            rotateCameraPosition(camera, "x", -1);
//        } else if (event.key.keysym.sym == SDLK_s) {
//            rotateCameraPosition(camera, "x", 1);
//        } else if (event.key.keysym.sym == SDLK_a) {
//            rotateCameraPosition(camera, "y", -1);
//        } else if (event.key.keysym.sym == SDLK_d) {
//            rotateCameraPosition(camera, "y", 1);
//        }
//    }
}

int main(int argc, char *argv[]) {
    // note: we are using a right hand coordinate system
    float scaleFactor = 0.35; // 0.35
    std::vector<ModelTriangle> modelTriangles = loadOBJ(scaleFactor, "cornell-box.obj");
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    glm::mat3 initialOrientation(1.0f);
    float focalLength = 2.0;
    Camera camera(initialPosition, initialOrientation, focalLength);
    camera.lookAt(glm::vec3(0.0, 0.0, 0.0));
//    glm::mat4 matrix = camera.buildMatrix();
//    std::cout << matrix[3][0] << "," << matrix [3][1] << "," << matrix[3][2] << std::endl;
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window, camera);
        draw(window, camera, modelTriangles);
        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
	}
}
