#ifndef REDNOISE_CAMERA_H
#define REDNOISE_CAMERA_H

#include "glm/glm.hpp"
#include <CanvasPoint.h>

enum Axis { x, y, z };

class Camera {
public:
    glm::vec3 position;
    glm::mat3 orientation;
    float focalLength;
    Camera(glm::vec3 position, glm::mat3 orientation, float focalLength);
    glm::mat4 buildMatrix();
    void translate(Axis axis, int sign);
    void rotate(Axis axis, int sign);
    void lookAt(glm::vec3 vertex);
    glm::vec3 getTransposedPoint(glm::vec3 vertex);
    CanvasPoint getIntersectionPoint(glm::vec3 vertex);
};


#endif //REDNOISE_CAMERA_H
