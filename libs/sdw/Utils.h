#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include "DrawingWindow.h"
#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"

#define WIDTH 480 // 320
#define HEIGHT 480 // 240

std::vector<std::string> split(const std::string &line, char delimiter);

std::vector<std::vector<float>> emptyDepthBuffer();
void rotatePosition(glm::vec3 &cameraPosition, std::string axis, int sign);
void translatePosition(glm::vec3 &cameraPosition, std::string axis, int sign);
glm::vec3 transposePoint(glm::vec3 &cameraPosition, glm::vec3 vertexPosition);
CanvasPoint getCanvasIntersectionPoint(glm::vec3 &cameraPosition, glm::vec3 vertexPosition, float focalLength);

std::vector<int> parseColourValue(std::string line);
std::string parseColourName(std::string line);
std::map<std::string, Colour> loadColours(std::string fileName);
glm::vec3 parseVector(std::string line, float scaleFactor);
int cleanFacet(std::string triangleRawItem);
std::vector<int> parseFacet(std::string line);
std::vector<ModelTriangle> createTriangles(std::vector<glm::vec3> trianglePoints, std::vector<std::vector<int>> triangles, std::vector<std::string> colours, std::map<std::string, Colour> colourMap);
std::vector<ModelTriangle> loadOBJ(float scaleFactor, std::string fileName);