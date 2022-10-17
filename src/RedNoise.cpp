#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include <Colour.h>
#include <Utils.h>
#include <fstream>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

#define WIDTH 1920 // 320
#define HEIGHT 1200 // 240

Colour randomColour() {
  int red = rand() % 256;
  int green = rand() % 256;
  int blue = rand() % 256;
  return Colour(red, green, blue);
}

CanvasPoint randomPoint() {
  float x = rand() % WIDTH;
  float y = rand() % HEIGHT;
  return CanvasPoint(x, y);
}

CanvasTriangle randomTriangle() {
  return CanvasTriangle({randomPoint(),  randomPoint(), randomPoint()});
}

void drawPixel(DrawingWindow &window, CanvasPoint point, Colour colour) {
  uint32_t colourCode = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
  window.setPixelColour(round(point.x), round(point.y), colourCode);
}

void drawLine(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour) {
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  float numSteps = std::max(abs(xDiff), abs(yDiff));
  float xStepSize = xDiff / numSteps;
  float yStepSize = yDiff / numSteps;
  for (float i=0.0; i<numSteps; i++) {
    float x = from.x + (xStepSize * i);
    float y = from.y + (yStepSize * i);
    drawPixel(window, CanvasPoint(x, y), colour);
  }
}

glm::vec3 parseVector(std::string line) {
  std::vector<std::string> trianglePointRaw = split(line, ' ');
  glm::vec3 trianglePoint = {std::stof(trianglePointRaw[1]), std::stof(trianglePointRaw[2]), std::stof(trianglePointRaw[3])};
  return trianglePoint;
}

int cleanFacet(std::string triangleRawItem) {
  return std::stof(triangleRawItem.substr(0, triangleRawItem.size()-1));
}

std::vector<int> parseFacet(std::string line) {
  std::vector<std::string> triangleRaw = split(line, ' ');
  std::vector<int> triangle = {cleanFacet(triangleRaw[1]), cleanFacet(triangleRaw[2]), cleanFacet(triangleRaw[3])};
  return triangle;
}

std::vector<ModelTriangle> createTriangles(std::vector<glm::vec3> trianglePoints, std::vector<std::vector<int>> triangles) {
  std::vector<ModelTriangle> modelTriangles;
  for (int i=0; i<triangles.size(); i++) {
    ModelTriangle modelTriangle = {trianglePoints[triangles[i][0] - 1], trianglePoints[triangles[i][1] - 1], trianglePoints[triangles[i][2] - 1], Colour()};
    modelTriangles.push_back(modelTriangle);
    std::cout << modelTriangle << std::endl;
  }
  return modelTriangles;
}

void loadOBJ() {
  std::vector<glm::vec3> trianglePoints;
  std::vector<std::vector<int>> triangles;
  std::ifstream filein("cornell-box.obj");
  for (std::string line; std::getline(filein, line); ) {
    if (line[0] == 'v') {
      trianglePoints.push_back(parseVector(line));
    } else if (line[0] == 'f') {
      triangles.push_back(parseFacet(line));
    }
  }
  std::vector<ModelTriangle> modelTriangles = createTriangles(trianglePoints, triangles);
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
  loadOBJ();
	// DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	// SDL_Event event;
	// while (true) {
	// 	// We MUST poll for events - otherwise the window will freeze !
	// 	if (window.pollForInputEvents(event)) handleEvent(event, window);
	// 	// Need to render the frame at the end, or nothing actually gets shown on the screen !
	// 	window.renderFrame();
}
