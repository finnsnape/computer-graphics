#pragma once

#include <vector>
#include <string>
#include <ModelTriangle.h>

namespace FilesUtils {
    std::vector<ModelTriangle> loadOBJ(float scaleFactor, std::string objFileName, std::string mtlFileName);
}