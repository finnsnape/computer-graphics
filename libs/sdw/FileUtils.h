#pragma once

#include "Colour.h"
#include "ModelTriangle.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <fstream>

std::vector<int> parseColourValue(std::string line);
std::string parseColourName(std::string line);
std::map<std::string, Colour> loadColours(std::string fileName);
glm::vec3 parseVector(std::string line, float scaleFactor);
int cleanFacet(std::string triangleRawItem);
std::vector<int> parseFacet(std::string line);
std::vector<ModelTriangle> createTriangles(std::vector<glm::vec3> trianglePoints, std::vector<std::vector<int>> triangles, std::vector<std::string> colours, std::map<std::string, Colour> colourMap);
std::vector<ModelTriangle> loadOBJ(float scaleFactor, std::string fileName);