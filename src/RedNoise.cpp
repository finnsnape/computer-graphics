#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
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

void drawStrokedTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour) {
  drawLine(window, triangle.v0(), triangle.v1(), colour);
  drawLine(window, triangle.v1(), triangle.v2(), colour);
  drawLine(window, triangle.v2(), triangle.v0(), colour);
}


float getClockwiseAngle(CanvasPoint point) {
  float angle = 0.0;
  angle = -1 * atan2(point.x, -1 * point.y);
  return angle;
}

bool comparePoints(CanvasPoint point1, CanvasPoint point2) {
  return getClockwiseAngle(point1) >= getClockwiseAngle(point2);
}

float area(int x1, int y1, int x2, int y2, int x3, int y3) {
  return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}
 
bool isInTriangle(CanvasTriangle triangle, CanvasPoint testPoint) {
  /* Calculate area of triangle ABC */
  float A = area (triangle.v0().x, triangle.v0().y, triangle.v1().x, triangle.v1().y, triangle.v2().x, triangle.v2().y);

  /* Calculate area of triangle PBC */ 
  float A1 = area (testPoint.x, testPoint.y, triangle.v1().x, triangle.v1().y, triangle.v2().x, triangle.v2().y);

  /* Calculate area of triangle PAC */ 
  float A2 = area (triangle.v0().x, triangle.v0().y, testPoint.x, testPoint.y, triangle.v2().x, triangle.v2().y);

  /* Calculate area of triangle PAB */  
  float A3 = area (triangle.v0().x, triangle.v0().y, triangle.v1().x, triangle.v1().y, testPoint.x, testPoint.y);
  
  /* Check if sum of A1, A2 and A3 is same as A */
  return (A == A1 + A2 + A3);
}

std::vector<float> boundingBox(CanvasTriangle triangle) {
  float minX = std::min({triangle.v0().x, triangle.v1().x, triangle.v2().x});
  float maxX = std::max({triangle.v0().x, triangle.v1().x, triangle.v2().x});
  float minY = std::min({triangle.v0().y, triangle.v1().y, triangle.v2().y});
  float maxY = std::max({triangle.v0().y, triangle.v1().y, triangle.v2().y});
  // std::cout << minX << "-" << maxX << " " << minY << "-" << maxY << std::endl;
  return {minX, maxX, minY, maxY};
}

void drawFilledTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour) {
  // we should be getting the smallest possible bounding box for the triangle rather than iterating over all of them
  std::vector<float> boundedBy = boundingBox(triangle);
  for (float x=boundedBy[0]; x<boundedBy[1]; x++) {
    for (float y=boundedBy[2]; y<boundedBy[3]; y++) {
      if (isInTriangle(triangle, CanvasPoint(x, y))) {
        drawPixel(window, CanvasPoint(x, y), colour);
      }
    }
  }
  drawStrokedTriangle(window, triangle, Colour(255, 255, 255));
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
    else if (event.key.keysym.sym == SDLK_u) {
      drawFilledTriangle(window, randomTriangle(), randomColour());
    }
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
