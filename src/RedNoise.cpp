#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include <Colour.h>
#include <Utils.h>
#include <TriangleUtils.h>
#include "../libs/sdw/3DUtils.h"
#include <FileUtils.h>
#include <glm/glm.hpp>
#include <vector>

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

void draw(DrawingWindow &window, glm::vec3 &cameraPosition, std::vector<ModelTriangle> modelTriangles) {
    window.clearPixels();
    float focalLength = 2.0;
    std::vector<std::vector<float>> depthBuffer = emptyDepthBuffer();
    for (int i=0; i<modelTriangles.size(); i++) {
	    ModelTriangle modelTriangle = modelTriangles[i];
	    std::vector<CanvasPoint> rawTriangle;
	    for (int j=0; j<3; j++) {
	        glm::vec3 vertex = modelTriangle.vertices[j];
	        CanvasPoint intersectionPoint = getCanvasIntersectionPoint(cameraPosition, vertex, focalLength);
	        // std::cout << intersectionPoint << std::endl;
	        rawTriangle.push_back(intersectionPoint);
	    }
	    CanvasTriangle triangle({rawTriangle[0], rawTriangle[1], rawTriangle[2]});
	    drawFilledTriangle(window, depthBuffer, triangle, modelTriangle.colour);
    }
}

void handleEvent(SDL_Event event, DrawingWindow &window, glm::vec3 &cameraPosition) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) {
            translatePosition(cameraPosition, "x", 1);
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            translatePosition(cameraPosition, "x", -1);
        } else if (event.key.keysym.sym == SDLK_UP) {
            translatePosition(cameraPosition, "y", -1);
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            translatePosition(cameraPosition, "y", 1);
        } else if (event.key.keysym.sym == SDLK_MINUS) {
            translatePosition(cameraPosition, "z", 1);
        } else if (event.key.keysym.sym == SDLK_EQUALS) {
            translatePosition(cameraPosition, "z", -1);
        } else if (event.key.keysym.sym == SDLK_w) {
            rotatePosition(cameraPosition, "y", -1);
        } else if (event.key.keysym.sym == SDLK_s) {
            rotatePosition(cameraPosition, "y", 1);
        } else if (event.key.keysym.sym == SDLK_a) {
            rotatePosition(cameraPosition, "x", -1);
        } else if (event.key.keysym.sym == SDLK_d) {
            rotatePosition(cameraPosition, "x", 1);
        }
    }
}

int main(int argc, char *argv[]) {
    float scaleFactor = 0.35; // 0.35
    std::vector<ModelTriangle> modelTriangles = loadOBJ(scaleFactor, "cornell-box.obj");
    glm::vec3 cameraPosition({0.0, 0.0, 4.0});
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window, cameraPosition);
        draw(window, cameraPosition, modelTriangles);
        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
	}
}
