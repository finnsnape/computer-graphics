#include "Scene.h"
#include "RayTracingUtils.h"
#include "RasterisingUtils.h"


Scene::Scene(float _width, float _height, std::vector<ModelTriangle> _triangles, Camera _camera):
    width(_width),
    height(_height),
    triangles(std::move(_triangles)),
    camera(std::move(_camera)) {
    this->window = DrawingWindow((int) width, (int) height, false);
}

Scene::Scene(float _width, float _height, std::vector<ModelTriangle> _triangles, Camera _camera, glm::vec3 _lightSource):
        width(_width),
        height(_height),
        triangles(std::move(_triangles)),
        camera(std::move(_camera)),
        lightSource(_lightSource) {
    this->window = DrawingWindow((int) width, (int) height, false);
}

void Scene::draw(Mode mode) {
    this->window.clearPixels();
    switch(mode) {
        case RAY_TRACED:
            RayTracingUtils::draw(*this);
            break;
        case RASTERISED:
            RasterisingUtils::drawFilled(*this);
            break;
        case WIRE_FRAME:
            RasterisingUtils::drawStroked(*this);
            break;
        default:
            break;
    }
}