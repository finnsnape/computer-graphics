#include "LightingUtils.h"
#include "Scene.h"
#include <cmath>
#include "RayTracingUtils.h"

namespace {
    Colour setIntensity(float intensity, RayTriangleIntersection &closestTriangle) {
        Colour colour = closestTriangle.intersectedTriangle.colour;
        colour.red *= intensity;
        colour.green *= intensity;
        colour.blue *= intensity;
        return colour;
    }

    /// @brief Gets brightness based on distance from light source
    float calculateProximityIntensity(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float distance = glm::length(scene.light.position - closestTriangle.intersectionPoint);
        float brightness = 1.f/(0.5f * glm::pi<float>() * pow(distance, 2));
        return fmin(brightness, 1.f);
    }

    float calculateIncidenceIntensity(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        glm::vec3 direction = glm::normalize(scene.light.position - closestTriangle.intersectionPoint); // point to light
        float angle = glm::dot(closestTriangle.intersectedTriangle.normal, direction);
        return fmax(angle, 0.f);
    }

    float calculateSpecularIntensity(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float specularStrength = 0.25f;
        glm::vec3 lightDir = glm::normalize(scene.light.position - closestTriangle.intersectionPoint); // point to light
        glm::vec3 viewDir = glm::normalize(scene.camera.position - closestTriangle.intersectionPoint);
        glm::vec3 reflectDir = glm::reflect(-lightDir, closestTriangle.intersectedTriangle.normal);
        float specFactor = fmax(glm::dot(viewDir, reflectDir), 0.f);
        float spec = pow(specFactor, 24.f);
        float intensity = specularStrength * spec;
        return fmax(intensity, 0.f);;
    }

    Colour applyProximityLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float intensity = calculateProximityIntensity(scene, closestTriangle);
        return setIntensity(intensity, closestTriangle);
    }

    Colour applySpecularLighting(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float brightness = calculateProximityIntensity(scene, closestTriangle);
        float ambient = 0.15f;
        float incidence = calculateIncidenceIntensity(scene, closestTriangle, useAmbient);
        float specular = calculateSpecularIntensity(scene, closestTriangle, useAmbient);
        float calculatedIntensity = brightness * (specular + 2.5f * incidence);
        if (!RayTracingUtils::canSeeLight(scene, closestTriangle)) calculatedIntensity = 0.f;
        float intensity = glm::clamp(ambient + calculatedIntensity, 0.f, 1.f);
        return setIntensity(intensity, closestTriangle);
    }
}

namespace LightingUtils {
    Colour applyLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        Colour colour;
        switch (scene.light.mode) {
            case Light::DEFAULT:
                colour = closestTriangle.intersectedTriangle.colour;
                break;
            case Light::PROXIMITY:
                colour = applyProximityLighting(scene, closestTriangle);
                break;
            case Light::SPECULAR:
                colour = applySpecularLighting(scene, closestTriangle, false);
                break;
            default:
                break;
        }
        return colour;
    }

    /// @brief Checks if the provided colour is equal to the object colour we have set to be a mirror
    bool isMirror(Colour &colour) {
        Colour mirrorColour(0, 255, 255);
        if (colour == mirrorColour) return true;
        return false;
    }

    Colour applyMirror(Scene &scene, RayTriangleIntersection &closestTriangle) {
        if (!isMirror(closestTriangle.intersectedTriangle.colour)) return closestTriangle.intersectedTriangle.colour;
        glm::vec3 viewDir = glm::normalize(scene.camera.position - closestTriangle.intersectionPoint);
        glm::vec3 reflectDir = glm::reflect(-viewDir, closestTriangle.intersectedTriangle.normal);
        Ray ray(closestTriangle.intersectionPoint, reflectDir);
        RayTriangleIntersection newClosestTriangle = RayTracingUtils::findClosestTriangle(scene, ray, true, closestTriangle.triangleIndex);
        if (newClosestTriangle.distanceFromCamera == FLT_MAX) {
            return {0, 0, 0}; // looking out into the ether
        }
        Colour colour = LightingUtils::applyLighting(scene, newClosestTriangle);
        float modifier = 0.85; // bit darker to make it more realistic
        // FIXME: can make this smarter by accounting for total distance (light -> triangle + triangle -> mirror + mirror -> eye)
        colour.red *= modifier;
        colour.green *= modifier;
        colour.blue *= modifier;
        return colour;
    }
}