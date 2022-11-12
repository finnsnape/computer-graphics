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

void drawRasterised(DrawingWindow &window, std::vector<std::vector<float>> &depthBuffer, glm::vec3 cameraPosition, float focalLength, std::vector<ModelTriangle> modelTriangles) {
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

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
  }
}

int main(int argc, char *argv[]) {
    float scaleFactor = 0.35; // 0.35
    glm::vec3 cameraPosition({0.0, 0.0, 4.0});
    float focalLength = 2.0;
    std::vector<std::vector<float>> depthBuffer = emptyDepthBuffer();
    std::vector<ModelTriangle> modelTriangles = loadOBJ(scaleFactor, "cornell-box.obj");
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    drawRasterised(window, depthBuffer, cameraPosition, focalLength, modelTriangles);
	    SDL_Event event;
	    while (true) {
		    // We MUST poll for events - otherwise the window will freeze !
		    if (window.pollForInputEvents(event)) handleEvent(event, window);
		    // Need to render the frame at the end, or nothing actually gets shown on the screen !
		    window.renderFrame();
    }
}
