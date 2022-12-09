#include "LightingUtils.h"
#include "Scene.h"
#include <cmath>
#include "RayTracingUtils.h"

namespace {
    /// @brief Gets brightness based on distance from light source
    float calculateBrightness(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float distance = glm::length(scene.lightSource - closestTriangle.intersectionPoint);
        float brightness = 1.f/(1.f * glm::pi<float>() * pow(distance, 2));
        return fmin(brightness, 1.f);
    }

    void setIntensity(float intensity, RayTriangleIntersection &closestTriangle) {
        Colour &colour = closestTriangle.intersectedTriangle.colour;
        colour.red *= intensity;
        colour.green *= intensity;
        colour.blue *= intensity;
    }

    void applyHardShadows(Scene &scene, RayTriangleIntersection &closestTriangle) {
        if (!RayTracingUtils::canSeeLight(scene, closestTriangle)) {
            closestTriangle.intersectedTriangle.colour = Colour(0, 0, 0);
        }
    }

    void applyProximityLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float intensity = calculateBrightness(scene, closestTriangle);
        setIntensity(intensity, closestTriangle);
    }

    void applyAngleOfIncidenceLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        // works well with 0,0.9,0.7
        float brightness = calculateBrightness(scene, closestTriangle);
        glm::vec3 direction = glm::normalize(scene.lightSource - closestTriangle.intersectionPoint); // point to light
        float angle = glm::dot(closestTriangle.intersectedTriangle.normal, direction);
        float ambient = 0.2; // min light amount
        float intensity = glm::clamp(angle * brightness * 4, ambient, 1.f);
        setIntensity(intensity, closestTriangle);
    }
}

namespace LightingUtils {
    void applyLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        switch (scene.lightingMode) {
            case Scene::DEFAULT:
                break;
            case Scene::HARD_SHADOWS:
                applyHardShadows(scene, closestTriangle);
                break;
            case Scene::PROXIMITY:
                applyProximityLighting(scene, closestTriangle);
                break;
            case Scene::ANGLE_OF_INCIDENCE:
                applyAngleOfIncidenceLighting(scene, closestTriangle);
                break;
            default:
                break;
        }
    }
}