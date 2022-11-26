#include "Scene.h"

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