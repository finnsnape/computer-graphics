#pragma once

#include <vector>
#include <string>
#include <map>
#include <Colour.h>
#include <ModelTriangle.h>
#include <fstream>

namespace FilesUtils {
    std::vector<ModelTriangle> loadOBJ(float scaleFactor, std::string objFileName, std::string mtlFileName);
}