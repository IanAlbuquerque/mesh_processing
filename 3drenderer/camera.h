#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Camera
{
private:
  glm::vec3 eye;
  glm::vec3 at;
  glm::vec3 up;
  float fovy;
  float zNear;
  float zFar;
  float width;
  float height;

public:
  Camera( glm::vec3 eye,
          glm::vec3 at,
          glm::vec3 up,
          float fovy,
          float zNear,
          float zFar,
          float width,
          float height);

  glm::mat4x4 getViewMatrix();
  glm::mat4x4 getProjectionMatrix();
  void updateWH(float width, float height);
};

#endif // CAMERA_H
