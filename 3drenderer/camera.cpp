#include "camera.h"
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( glm::vec3 eye,
                glm::vec3 at,
                glm::vec3 up,
                float fovy,
                float zNear,
                float zFar,
                float width,
                float height)
{
  this->eye = eye;
  this->at = at;
  this->up = up;
  this->fovy = fovy;
  this->zNear = zNear;
  this->zFar = zFar;
  this->width = width;
  this->height = height;
}

glm::mat4x4 Camera::getViewMatrix()
{
  return glm::lookAt(this->eye, this->at, this->up);
}

glm::mat4x4 Camera::getProjectionMatrix()
{
  return glm::perspective(glm::radians(this->fovy), this->width / this->height, this->zNear, this->zFar);
}

void Camera::updateWH(float width, float height)
{
  this->width = width;
  this->height = height;
}
