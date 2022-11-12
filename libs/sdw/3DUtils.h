#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "CanvasPoint.h"

std::vector<std::vector<float>> emptyDepthBuffer();
void rotatePosition(glm::vec3 &cameraPosition, std::string axis, int sign);
void translatePosition(glm::vec3 &cameraPosition, std::string axis, int sign);
glm::vec3 transposePoint(glm::vec3 &cameraPosition, glm::vec3 vertexPosition);
CanvasPoint getCanvasIntersectionPoint(glm::vec3 &cameraPosition, glm::vec3 vertexPosition, float focalLength);