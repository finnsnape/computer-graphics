#include "LightingUtils.h"
#include "Scene.h"
#include <cmath>
#include "RayTracingUtils.h"

namespace {
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

    /// @brief Gets brightness based on distance from light source
    float calculateProximityIntensity(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float distance = glm::length(scene.lightSource - closestTriangle.intersectionPoint);
        float brightness = 1.f/(0.5f * glm::pi<float>() * pow(distance, 2));
        return fmin(brightness, 1.f);
    }

    float calculateIncidenceIntensity(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float brightness = calculateProximityIntensity(scene, closestTriangle);
        glm::vec3 direction = glm::normalize(scene.lightSource - closestTriangle.intersectionPoint); // point to light
        float angle = glm::dot(closestTriangle.intersectedTriangle.normal, direction);
        return fmax(angle, 0.f);
        //float ambient = 0.f; // min light amount
        //if (useAmbient) ambient = 0.3f;
        //float intensity = glm::clamp(angle * brightness * 4, 0.3f, 1.f);
        //return intensity;
    }

    float calculateSpecularIntensity(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float specularStrength = 0.5f;
        glm::vec3 lightDir = glm::normalize(scene.lightSource - closestTriangle.intersectionPoint); // point to light
        glm::vec3 viewDir = glm::normalize(scene.camera.position - closestTriangle.intersectionPoint);
        glm::vec3 reflectDir = glm::reflect(-lightDir, closestTriangle.intersectedTriangle.normal);
        //std::cout << glm::dot(viewDir, reflectDir) << std::endl;
        float specFactor = fmax(glm::dot(viewDir, reflectDir), 0.f);
        float spec = pow(specFactor, 64);
        float intensity = specularStrength * spec;
        return fmax(intensity, 0.f);
    }

    void applyAngleOfIncidenceLighting(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        // works well with 0,0.9,0.7
        float intensity = calculateIncidenceIntensity(scene, closestTriangle, useAmbient);
        setIntensity(intensity, closestTriangle);
    }


    void applyProximityLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float intensity = calculateProximityIntensity(scene, closestTriangle);
        setIntensity(intensity, closestTriangle);
    }

    void applySpecularLighting(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float brightness = calculateProximityIntensity(scene, closestTriangle);
        float ambient = 0.15f;
        float incidence = calculateIncidenceIntensity(scene, closestTriangle, useAmbient);
        float specular = calculateSpecularIntensity(scene, closestTriangle, useAmbient);
        if (incidence > 1) std::cout << incidence;
        //std::cout << "specular: " << specular << std::endl;
        //std::cout << "indidence: " << incidence << std::endl;
        //std::cout << "brightness: " << brightness << std::endl;
        float intensity = glm::clamp(specular + ambient + brightness * 4 * incidence, 0.f, 1.f);
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
                applyAngleOfIncidenceLighting(scene, closestTriangle, false);
                break;
            case Scene::AMBIENT:
                applyAngleOfIncidenceLighting(scene, closestTriangle, true);
                break;
            case Scene::SPECULAR:
                applySpecularLighting(scene, closestTriangle, false);
                break;
            default:
                break;
        }
    }
}