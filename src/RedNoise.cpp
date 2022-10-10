#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <CanvasPoint.h>
#include <Colour.h>
#include <Utils.h>
#include <fstream>

#define WIDTH 1920 // 320
#define HEIGHT 1200 // 240

void drawLine(DrawingWindow &window, CanvasPoint from, CanvasPoint to, Colour colour) {
  float xDiff = to.x - from.x;
  float yDiff = to.y - from.y;
  float numSteps = std::max(abs(xDiff), abs(yDiff));
  float xStepSize = xDiff / numSteps;
  float yStepSize = yDiff / numSteps;
  uint32_t colourCode = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
  for (float i=0.0; i<numSteps; i++) {
    float x = from.x + (xStepSize * i);
    float y = from.y + (yStepSize * i);
    window.setPixelColour(round(x), round(y), colourCode);
  }
}

void draw(DrawingWindow &window) {
	window.clearPixels();
  drawLine(window, CanvasPoint(0, 0), CanvasPoint(100, 100), Colour(255, 255, 255));
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
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
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
