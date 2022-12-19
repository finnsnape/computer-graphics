#include "Scene.h"
#include "RayTracingUtils.h"
#include "RasterisingUtils.h"
#include "TriangleUtils.h"

Scene::Scene(float _width, float _height, bool _show, bool _mirror, RenderMode _renderMode, Light _light, std::vector<ModelTriangle> _triangles, Camera _camera):
        width(_width),
        height(_height),
        show(_show),
        mirror(_mirror),
        renderMode(_renderMode),
        light(_light),
        triangles(std::move(_triangles)),
        camera(std::move(_camera))
        {
    this->window = DrawingWindow((int) width, (int) height, false, show);
    this->modelToWorld();
    this->calculateNormals();
}

/// @brief Converts the loaded model coordinates to world coordinates
void Scene::modelToWorld() {
    for (auto &triangle : this->triangles) {
        for (auto &vertex : triangle.vertices) {
            glm::vec4 temp(vertex, 1.f);
            vertex = glm::vec3(this->camera.model * temp);
        }
    }
}

void Scene::calculateNormals() {
    for (auto &triangle : this->triangles) {
        triangle.surfaceNormal = TriangleUtils::calculateSurfaceNormal(triangle.vertices);
        std::cout << triangle.surfaceNormal.x << "," << triangle.surfaceNormal.y << "," << triangle.surfaceNormal.z << std::endl;
    }
    std::cout << "=======" << std::endl;
    for (auto &triangle : this->triangles) {
        std::cout << triangle.vertexNormals.size() << std::endl;
        triangle.vertexNormals[0] = TriangleUtils::calculateVertexNormals(triangle, this->triangles, triangle.vertices[0]);
        triangle.vertexNormals[1] = TriangleUtils::calculateVertexNormals(triangle, this->triangles, triangle.vertices[1]);
        triangle.vertexNormals[2] = TriangleUtils::calculateVertexNormals(triangle, this->triangles, triangle.vertices[2]);

        /// FIXME 1, 2, 3 ????????????????????????????
        std::cout << triangle.vertexNormals[1].x << "," << triangle.vertexNormals[1].y << "," << triangle.vertexNormals[1].z << std::endl;
        std::cout << triangle.vertexNormals[2].x << "," << triangle.vertexNormals[2].y << "," << triangle.vertexNormals[2].z << std::endl;
        std::cout << triangle.vertexNormals[0].x << "," << triangle.vertexNormals[3].y << "," << triangle.vertexNormals[3].z << std::endl;


    }
}

/// @brief Moves the light around in the scene
void Scene::moveLight(Camera::Axis axis, float sign) {
    float delta = 0.1f * sign;
    switch(axis) {
        case Camera::Axis::x:
            this->light.position.x += delta;
            break;
        case Camera::Axis::y:
            this->light.position.y += delta;
            break;
        case Camera::Axis::z:
            this->light.position.z += delta;
            break;
        default:
            break;
    }
    std::cout << "light: " << light.position.x << "," << light.position.y << "," << light.position.z << std::endl;

}

void Scene::draw() {
    this->window.clearPixels();
    switch(this->renderMode) {
        case WIRE_FRAME:
            RasterisingUtils::drawStroked(*this);
            break;
        case RASTERISED:
            RasterisingUtils::drawFilled(*this);
            break;
        case RAY_TRACED:
            RayTracingUtils::draw(*this);
            break;
        default:
            break;
    }
    // FIXME: temp to draw light pos
    RasterisingUtils::a(*this);
}