#include "LightingUtils.h"
#include "Scene.h"
#include <cmath>

namespace {
    void applyHardShadows(RayTriangleIntersection &closestTriangle) {
        closestTriangle.intersectedTriangle.colour = Colour(0, 0, 0);
    }

    void applyProximityLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        Colour &colour = closestTriangle.intersectedTriangle.colour;
        float distance = glm::length(closestTriangle.intersectionPoint - scene.lightSource);
        float intensity = 1.f/(1 * glm::pi<float>() * pow(distance, 2));
        intensity = std::min(intensity, 1.f);
        colour.red *= intensity;
        colour.green *= intensity;
        colour.blue *= intensity;
    }

    void applyAngleOfIncidenceLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        Colour &colour = closestTriangle.intersectedTriangle.colour;
        glm::vec3 direction = glm::normalize(scene.lightSource - closestTriangle.intersectionPoint); // point to light
        float angle = glm::dot(glm::normalize(closestTriangle.intersectedTriangle.normal), direction);
        float intensity = abs(angle);
        if (intensity >= 1 || intensity <= 0) {
            std::cout << intensity << std::endl;
        }
        colour.red *= intensity;
        colour.green *= intensity;
        colour.blue *= intensity;
    }
}

namespace ShadowUtils {
    void applyShadows(Scene &scene, RayTriangleIntersection &closestTriangle) {
        switch (scene.mode) {
            case Scene::RAY_TRACED_HARD:
                applyHardShadows(closestTriangle);
                break;
            case Scene::RAY_TRACED_PROXIMITY:
                applyProximityLighting(scene, closestTriangle);
                break;
            case Scene::RAY_TRACED_INCIDENCE:
                applyAngleOfIncidenceLighting(scene, closestTriangle);
                break;
            default:
                break;
        }
    }
}