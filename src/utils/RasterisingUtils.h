#pragma once

class Scene; // pre-declare to avoid circular dependency
class Camera;

namespace RasterisingUtils {
    void drawFilled(Scene &scene);
    void drawStroked(Scene &scene);
}