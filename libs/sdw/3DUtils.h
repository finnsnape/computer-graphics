#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "CanvasPoint.h"

std::vector<std::vector<float>> emptyDepthBuffer();
glm::vec3 transposePoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition);
CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength);