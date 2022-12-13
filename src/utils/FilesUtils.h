#pragma once

#include <vector>
#include <string>
#include <ModelTriangle.h>
#include <DrawingWindow.h>

namespace FilesUtils {
    std::vector<ModelTriangle> loadOBJ(float scaleFactor, std::string objFileName, std::string mtlFileName);
    void saveAsImage(DrawingWindow &window);
}