#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <vector>
#include "Triangles.h"
#include "Files.h"
#include "Camera.h"

#define WIDTH 480
#define HEIGHT 480

void drawRayTraced(DrawingWindow &window, Camera &camera, std::vector<ModelTriangle> &modelTriangles) {
    window.clearPixels();
    for (int x=0; x<WIDTH; x++) {
        for (int y=0; y<HEIGHT; y++) {
            CanvasPoint canvasPoint((float) x, (float) y);
            glm::vec3 rayDirection = camera.getRayDirection(canvasPoint);
            RayTriangleIntersection solution = camera.getClosestIntersection(modelTriangles, rayDirection);
            if (solution.exists) {
                drawPixel(window, canvasPoint, solution.intersectedTriangle.colour);
            }
        }
    }
}

void handleEvent(SDL_Event event, DrawingWindow &window, Camera &camera) {
//	if (event.type == SDL_KEYDOWN) {
//		if (event.key.keysym.sym == SDLK_LEFT) {
//            camera.translate(x, -1);
//        } else if (event.key.keysym.sym == SDLK_RIGHT) {
//            camera.translate(x, 1);
//        } else if (event.key.keysym.sym == SDLK_UP) {
//            camera.translate(y, 1);
//        } else if (event.key.keysym.sym == SDLK_DOWN) {
//            camera.translate(y, -1);
//        } else if (event.key.keysym.sym == SDLK_MINUS) {
//            camera.translate(z, 1);
//        } else if (event.key.keysym.sym == SDLK_EQUALS) {
//            camera.translate(z, -1);
//        } else if (event.key.keysym.sym == SDLK_w) {
//            camera.rotate(x, -1);
//        } else if (event.key.keysym.sym == SDLK_s) {
//            camera.rotate(x, 1);
//        } else if (event.key.keysym.sym == SDLK_a) {
//            camera.rotate(y, -1);
//        } else if (event.key.keysym.sym == SDLK_d) {
//            camera.rotate(y, 1);
//        }
//        camera.lookAt({0.0, 0.0, 0.0});
//    }
}

int main(int argc, char *argv[]) {
    float scaleFactor = 0.35;
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    float focalLength = 2.0;
    Camera camera(initialPosition, focalLength);
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    std::vector<ModelTriangle> modelTriangles = loadOBJ(scaleFactor, "cornell-box.obj");
    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window, camera);
        drawRayTraced(window, camera, modelTriangles);
        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
	}
}
