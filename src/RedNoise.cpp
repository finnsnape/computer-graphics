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
#include <map>

#define WIDTH 240 // 320
#define HEIGHT 240 // 240

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

glm::vec3 transposePoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition) {
  return {cameraPosition.x-vertexPosition.x, cameraPosition.y-vertexPosition.y, cameraPosition.z-vertexPosition.z};
}

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength) {
  // note +z is towards you, -z is away from you
  glm::vec3 transposedVertexPosition = transposePoint(cameraPosition, vertexPosition);
  float u = focalLength * (HEIGHT/2) * (transposedVertexPosition.x/transposedVertexPosition.z) + WIDTH/2;
  float v = focalLength * (HEIGHT/2) * (transposedVertexPosition.y/transposedVertexPosition.z) + HEIGHT/2;
  return CanvasPoint(u, v);
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

void drawPointCloud(DrawingWindow &window, glm::vec3 cameraPosition, float focalLength, std::vector<glm::vec3> trianglePoints) {
  for (int i=0; i<trianglePoints.size(); i++) {
    CanvasPoint intersectionPoint = getCanvasIntersectionPoint(cameraPosition, trianglePoints[i], focalLength);
    drawPixel(window, intersectionPoint, Colour(255, 255, 255));
  }
}

std::vector<int> parseColourValue(std::string line) {
  std::vector<std::string> colourRaw = split(line, ' ');
  std::vector<int> colour = {round(std::stof(colourRaw[1]) * 255), round(std::stof(colourRaw[2]) * 255), round(std::stof(colourRaw[3]) * 255)};
  return colour;
}

std::string parseColourName(std::string line) {
  return split(line, ' ')[1];
}

std::map<std::string, Colour> loadColours() {
  std::map<std::string, Colour> colourMap;
  std::vector<std::string> colourNames;
  std::vector<std::vector<int>> colourValues;
  std::ifstream filein("cornell-box.mtl");
  for (std::string line; std::getline(filein, line); ) {
    if (line[0] == 'n') {
      colourNames.push_back(parseColourName(line));
    } else if (line[0] == 'K') {
      colourValues.push_back(parseColourValue(line));
    }
  }
  for (int i=0; i<colourNames.size(); i++) {
    Colour colourValue(colourValues[i][0], colourValues[i][1], colourValues[i][2]);
    colourMap.insert({colourNames[i], colourValue});
  }
  return colourMap;
}

glm::vec3 parseVector(std::string line, float scaleFactor) {
  std::vector<std::string> trianglePointRaw = split(line, ' ');
  glm::vec3 trianglePoint = {std::stof(trianglePointRaw[1]) * scaleFactor, std::stof(trianglePointRaw[2]) * scaleFactor, std::stof(trianglePointRaw[3]) * scaleFactor};
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

std::vector<ModelTriangle> createTriangles(std::vector<glm::vec3> trianglePoints, std::vector<std::vector<int>> triangles, std::vector<std::string> colours, std::map<std::string, Colour> colourMap) {
  std::vector<ModelTriangle> modelTriangles;
  for (int i=0; i<triangles.size(); i++) {
    Colour colour = colourMap.at(colours[i]);
    ModelTriangle modelTriangle = {trianglePoints[triangles[i][0] - 1], trianglePoints[triangles[i][1] - 1], trianglePoints[triangles[i][2] - 1], colour};
    modelTriangles.push_back(modelTriangle);
    //std::cout << modelTriangle << " " << colour << std::endl;
  }
  return modelTriangles;
}

std::vector<glm::vec3> loadOBJ(float scaleFactor) {
  std::vector<glm::vec3> trianglePoints;
  std::vector<std::vector<int>> triangles;
  std::vector<std::string> colours;
  std::map<std::string, Colour> colourMap = loadColours();
  std::ifstream filein("cornell-box.obj");
  std::string lastColourName;
  for (std::string line; std::getline(filein, line); ) {
    if (line[0] == 'v') {
      glm::vec3 trianglePoint = parseVector(line, scaleFactor);
      trianglePoints.push_back(trianglePoint);
    } else if (line[0] == 'f') {
      triangles.push_back(parseFacet(line));
      colours.push_back(lastColourName);
    } else if (line[0] == 'u') {
      lastColourName = parseColourName(line);
    }
  }
  //std::vector<ModelTriangle> modelTriangles = createTriangles(trianglePoints, triangles, colours, colourMap);
  return trianglePoints;
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
  std::vector<glm::vec3> trianglePoints = loadOBJ(scaleFactor);
  //loadColours();
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
  drawPointCloud(window, cameraPosition, focalLength, trianglePoints);
	SDL_Event event;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
  }
}
