#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#define WIDTH 1920 // 320
#define HEIGHT 1200 // 240

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues) {
  float difference = to - from;
  float delta = difference / (numberOfValues - 1);
  std::vector<float> interpolatedFloats;
  for (int i=0; i<numberOfValues; i++) interpolatedFloats.push_back(from + (i * delta));
  return interpolatedFloats;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues) {
  std::vector<std::vector<float>> temp;
  for (int i=0; i<3; i++) {
    std::vector<float> vec = interpolateSingleFloats(from[i], to[i], numberOfValues);
    temp.push_back(vec);
  }

  std::vector<glm::vec3> interpolatedThreeElementValues;
  for (int i=0; i<numberOfValues; i++) {
    glm::vec3 vec(temp[0][i], temp[1][i], temp[2][i]);
    interpolatedThreeElementValues.push_back(vec);
  }
  return interpolatedThreeElementValues;
}

void draw(DrawingWindow &window) {
	window.clearPixels();
  glm::vec3 topLeft(255, 0, 0);
  glm::vec3 topRight(0, 0, 255);
  glm::vec3 bottomRight(0, 255, 0);
  glm::vec3 bottomLeft(255, 255, 0);
  std::vector<glm::vec3> firstColumn = interpolateThreeElementValues(topLeft, bottomLeft, window.height);
  std::vector<glm::vec3> lastColumn = interpolateThreeElementValues(topRight, bottomRight, window.height);
	for (size_t y = 0; y < window.height; y++) {
    std::vector<glm::vec3> colourRow = interpolateThreeElementValues(firstColumn[y], lastColumn[y], window.width);
		for (size_t x = 1; x < window.width - 1; x++) {
			float red = colourRow[x][0];
			float green = colourRow[x][1];
			float blue = colourRow[x][2];
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
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
