#pragma once

#include <glm/glm.hpp>
#include <CanvasPoint.h>

class Scene; // pre-declare to avoid circular dependency

namespace RasterisingUtils {
    void drawFilled(Scene &scene);
    void drawStroked(Scene &scene);
    void a(Scene &scene); // FIXME: temp
}