#include "Scene.h"
#include "RayTracingUtils.h"
#include "RasterisingUtils.h"


Scene::Scene(float _width, float _height, Mode _mode, std::vector<ModelTriangle> _triangles, Camera _camera):
        width(_width),
        height(_height),
        mode(_mode),
        triangles(std::move(_triangles)),
        camera(std::move(_camera)) {
    this->window = DrawingWindow((int) width, (int) height, false);
}

Scene::Scene(float _width, float _height, Mode _mode, std::vector<ModelTriangle> _triangles, Camera _camera, glm::vec3 _lightSource):
        width(_width),
        height(_height),
        mode(_mode),
        triangles(std::move(_triangles)),
        camera(std::move(_camera)),
        lightSource(_lightSource) {
    this->window = DrawingWindow((int) width, (int) height, false);
}

void Scene::moveLight(Camera::Axis axis, int sign) {
    float modifier = 0.1f * sign;
    switch(axis) {
        case Camera::Axis::x:
            this->lightSource.x += modifier;
            break;
        case Camera::Axis::y:
            this->lightSource.y += modifier;
            break;
        case Camera::Axis::z:
            this->lightSource.z += modifier;
            break;
        default:
            break;
    }
}

void Scene::draw() {
    this->window.clearPixels();
    switch(this->mode) {
        case WIRE_FRAME:
            RasterisingUtils::drawStroked(*this);
            break;
        case RASTERISED:
            RasterisingUtils::drawFilled(*this);
            break;
        case RAY_TRACED:
            RayTracingUtils::draw(*this);
            break;
        case RAY_TRACED_HARD:
            RayTracingUtils::draw(*this);
            break;
        case RAY_TRACED_PROXIMITY:
            RayTracingUtils::draw(*this);
            break;
        default:
            break;
    }
    if (this->camera.orbit) {
        this->camera.rotate(Camera::y, 1);
    }
}