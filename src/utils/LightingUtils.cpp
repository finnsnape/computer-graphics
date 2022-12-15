#include "LightingUtils.h"
#include "Scene.h"
#include <cmath>
#include "RayTracingUtils.h"

namespace {
    Colour setIntensity(float intensity, RayTriangleIntersection closestTriangle) {
        Colour colour = closestTriangle.intersectedTriangle.colour;
        colour.red *= intensity;
        colour.green *= intensity;
        colour.blue *= intensity;
        return colour;
    }

    Colour applyHardShadows(Scene &scene, RayTriangleIntersection closestTriangle) {
        if (!RayTracingUtils::canSeeLight(scene, closestTriangle)) {
            return Colour(0, 0, 0);
        }
        return closestTriangle.intersectedTriangle.colour;
    }

    /// @brief Gets brightness based on distance from light source
    float calculateProximityIntensity(Scene &scene, RayTriangleIntersection &closestTriangle) {
        float distance = glm::length(scene.light.position - closestTriangle.intersectionPoint);
        float brightness = 1.f/(0.5f * glm::pi<float>() * pow(distance, 2));
        return fmin(brightness, 1.f);
    }

    float calculateIncidenceIntensity(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float brightness = calculateProximityIntensity(scene, closestTriangle);
        glm::vec3 direction = glm::normalize(scene.light.position - closestTriangle.intersectionPoint); // point to light
        float angle = glm::dot(closestTriangle.intersectedTriangle.normal, direction);
        return fmax(angle, 0.f);
        //float ambient = 0.f; // min light amount
        //if (useAmbient) ambient = 0.3f;
        //float intensity = glm::clamp(angle * brightness * 4, 0.3f, 1.f);
        //return intensity;
    }

    float calculateSpecularIntensity(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        float specularStrength = 0.5f;
        glm::vec3 lightDir = glm::normalize(scene.light.position - closestTriangle.intersectionPoint); // point to light
        glm::vec3 viewDir = glm::normalize(scene.camera.position - closestTriangle.intersectionPoint);
        glm::vec3 reflectDir = glm::reflect(-lightDir, closestTriangle.intersectedTriangle.normal);
        //std::cout << glm::dot(viewDir, reflectDir) << std::endl;
        float specFactor = fmax(glm::dot(viewDir, reflectDir), 0.f);
        float spec = pow(specFactor, 64.f);
        float intensity = specularStrength * spec;
        return fmax(intensity, 0.f);
    }

    /// @brief Checks if the provided colour is equal to the object colour we have set to be a mirror
    bool isMirror(Colour colour) {
        Colour mirrorColour(255, 0, 0);
        if (colour == mirrorColour) return true;
        return false;
    }

    Colour applyMirror(Scene &scene, RayTriangleIntersection closestTriangle) {
        if (!isMirror(closestTriangle.intersectedTriangle.colour)) return closestTriangle.intersectedTriangle.colour;
        glm::vec3 viewDir = glm::normalize(scene.camera.position - closestTriangle.intersectionPoint);
        glm::vec3 reflectDir = glm::reflect(-viewDir, closestTriangle.intersectedTriangle.normal);
        Ray ray(closestTriangle.intersectionPoint, reflectDir);
        RayTriangleIntersection newClosestTriangle = RayTracingUtils::findClosestTriangle(scene, ray, true, closestTriangle.triangleIndex);
        if (newClosestTriangle.distanceFromCamera == FLT_MAX) {
            //closestTriangle.intersectedTriangle.colour = Colour(0, 0, 0);
            return Colour(0, 0, 0);
        }
        return newClosestTriangle.intersectedTriangle.colour;
        // FIXME: maybe we shouldn't actually update the colour, just set the pixel?
        // FIXME: perhaps incorrect normals are causing weird reflections?
        // FIXME: maybe we need to get the next triangle that's not this one?
    }

    Colour applyAngleOfIncidenceLighting(Scene &scene, RayTriangleIntersection &closestTriangle, bool useAmbient) {
        // works well with 0,0.9,0.7
        float intensity = calculateIncidenceIntensity(scene, closestTriangle, useAmbient);
        return setIntensity(intensity, closestTriangle);
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
        if (incidence > 1) std::cout << incidence;
        float intensity = glm::clamp(specular + ambient + brightness * 4 * incidence, 0.f, 1.f);
        return setIntensity(intensity, closestTriangle);
    }
}

namespace LightingUtils {
    // FIXME: we're setting the colour of entire triangles as opposed to pixels
    // should do this individually. however, may complicate the mirror process
    Colour applyLighting(Scene &scene, RayTriangleIntersection &closestTriangle) {
        Colour colour;
        switch (scene.light.mode) {
            case Light::DEFAULT:
                colour = applyMirror(scene, closestTriangle);
                break;
            case Light::HARD_SHADOWS:
                colour = applyHardShadows(scene, closestTriangle);
                break;
            case Light::PROXIMITY:
                colour = applyProximityLighting(scene, closestTriangle);
                break;
            case Light::ANGLE_OF_INCIDENCE:
                colour = applyAngleOfIncidenceLighting(scene, closestTriangle, false);
                break;
            case Light::AMBIENT:
                colour = applyAngleOfIncidenceLighting(scene, closestTriangle, true);
                break;
            case Light::SPECULAR:
                colour = applySpecularLighting(scene, closestTriangle, false);
                break;
            default:
                break;
        }
        return colour;
    }
}