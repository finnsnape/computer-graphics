#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <ModelTriangle.h>
#include <glm/glm.hpp>
#include <vector>
#include "utils/Triangles.h"
#include "utils/Files.h"
#include "utils/Camera.h"

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

void draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle>& modelTriangles) {
    window.clearPixels();
    camera.resetDepthBuffer();
    for (auto modelTriangle : modelTriangles) {
        std::vector<CanvasPoint> rawTriangle;
        for (int j = 0; j < 3; j++) {
            glm::vec3 vertex = modelTriangle.vertices[j];
            CanvasPoint intersectionPoint = camera.getIntersectionPoint(vertex);
            rawTriangle.push_back(intersectionPoint);
        }
        CanvasTriangle triangle({rawTriangle[0], rawTriangle[1], rawTriangle[2]});
        drawFilledTriangle(window, camera.depthBuffer, triangle, modelTriangle.colour);
    }
    camera.orbit(x, 1, {0.0, 0.0, 0.0});
}

void handleEvent(SDL_Event event, DrawingWindow &window, Camera &camera) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) {
            camera.translate(x, -1);
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            camera.translate(x, 1);
        } else if (event.key.keysym.sym == SDLK_UP) {
            camera.translate(y, 1);
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            camera.translate(y, -1);
        } else if (event.key.keysym.sym == SDLK_MINUS) {
            camera.translate(z, 1);
        } else if (event.key.keysym.sym == SDLK_EQUALS) {
            camera.translate(z, -1);
        } else if (event.key.keysym.sym == SDLK_w) {
            camera.rotate(x, -1);
        } else if (event.key.keysym.sym == SDLK_s) {
            camera.rotate(x, 1);
        } else if (event.key.keysym.sym == SDLK_a) {
            camera.rotate(y, -1);
        } else if (event.key.keysym.sym == SDLK_d) {
            camera.rotate(y, 1);
        }
        camera.lookAt({0.0, 0.0, 0.0});
    }
}

int main(int argc, char *argv[]) {
    // note: we are using a right hand coordinate system
    float scaleFactor = 0.35; // 0.35
    glm::vec3 initialPosition(0.0, 0.0, 4.0);
    float focalLength = 2.0;
    Camera camera(initialPosition, focalLength);
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    std::vector<ModelTriangle> modelTriangles = loadOBJ(scaleFactor, "cornell-box.obj");
    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window, camera);
        draw(window, camera, modelTriangles);
        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
	}
}
