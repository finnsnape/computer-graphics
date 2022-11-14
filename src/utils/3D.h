#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <CanvasPoint.h>

std::vector<std::vector<float>> emptyDepthBuffer();
void rotateCameraPosition(glm::mat4 &camera, std::string axis, int sign);
void translateCameraPosition(glm::mat4 &camera, std::string axis, int sign);
glm::vec3 transposePoint(glm::mat4 &camera, glm::vec3 vertexPosition);
CanvasPoint getCanvasIntersectionPoint(glm::mat4 &camera, glm::vec3 vertexPosition, float focalLength);
