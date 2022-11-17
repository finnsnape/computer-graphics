#ifndef REDNOISE_CAMERA_H
#define REDNOISE_CAMERA_H

#include "glm/glm.hpp"
#include <CanvasPoint.h>
#include <vector>

enum Axis { x, y, z };

class Camera {
private:
    float focalLength;
    glm::vec3 getTransposedPoint(glm::vec3 vertex) const;
public:
    glm::vec3 position;
    glm::mat3 orientation;
    std::vector<std::vector<float>> depthBuffer;
    Camera(glm::vec3 position, float focalLength);
    void resetDepthBuffer();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
    CanvasPoint getIntersectionPoint(glm::vec3 vertex);
    void orbit(Axis axis, int sign, glm::vec3 vertex);
};


#endif //REDNOISE_CAMERA_H
