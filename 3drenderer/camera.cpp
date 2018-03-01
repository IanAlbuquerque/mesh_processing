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

  this->zoom = 1.0f;
  this->ensureZoomInRange();
}

glm::mat4x4 Camera::getViewMatrix()
{
  return glm::lookAt(this->eye, this->at, this->up);
}

glm::mat4x4 Camera::getProjectionMatrix()
{
  return glm::perspective( this->getFovYWithZoom(),
                           this->width / this->height,
                           this->zNear,
                           this->zFar);
}

float Camera::getFovYWithZoom()
{
  return glm::atan(glm::tan(glm::radians(this->fovy)) / this->zoom);
}

float Camera::getFovXWithZoom()
{
  return 2.0f * glm::atan(this->width / this->height * glm::tan( this->getFovYWithZoom() / 2.0f ));
}

void Camera::updateWH(float width, float height)
{
  this->width = width;
  this->height = height;
}

glm::vec3 Camera::getPosition()
{
  return this->eye;
}

glm::vec3 Camera::getFrontUnitVector()
{
  return glm::normalize(this->at - this->eye);
}

glm::vec3 Camera::getBackUnitVector()
{
  return -1.0f * this->getFrontUnitVector();
}

glm::vec3 Camera::getRightUnitVector()
{
  return glm::cross(this->getFrontUnitVector(), glm::normalize(this->up));
}

glm::vec3 Camera::getLeftUnitVector()
{
  return -1.0f * this->getRightUnitVector();
}

glm::vec3 Camera::getUpUnitVector()
{
  return glm::cross(this->getRightUnitVector(), this->getFrontUnitVector());
}

glm::vec3 Camera::getDownUnitVector()
{
  return -1.0f * this->getUpUnitVector();
}

glm::vec3 Camera::getAboveUnitVector()
{
  return glm::normalize(this->up);
}

glm::vec3 Camera::getBelowUnitVector()
{
  return -1.0f * this->getAboveUnitVector();
}

void Camera::lookAt(glm::vec3 point)
{
  this->at = point;
}

void Camera::lookDelta(glm::vec3 delta)
{
  this->at += delta;
}

void Camera::lookRotateHorizontal(float angle)
{
  glm::vec3 newAt = this->at;
  newAt -= this->eye;
  this->at = glm::rotate(newAt, glm::radians(angle), this->up) + this->eye;
}

void Camera::lookRotateVertical(float angle)
{
  glm::vec3 newAt = this->at;
  newAt -= this->eye;
  glm::vec3 newAtNormalized = glm::normalize(newAt);
  glm::vec3 upNormalized = glm::normalize(this->up);
  float currentAngle = glm::degrees(glm::acos(glm::dot(newAtNormalized, upNormalized)));
  if (currentAngle - angle < 1.0f)
  {
    angle = currentAngle - 1.0f;
  }
  if (currentAngle - angle > 180.0f - 1.0f)
  {
    angle = currentAngle - (180.0f - 1.0f);
  }
  this->at = glm::rotate(newAt, glm::radians(angle), this->getRightUnitVector()) + this->eye;
}

void Camera::moveTo(glm::vec3 point)
{
  this->at += point - this->eye;
  this->eye = point;
}

void Camera::moveDelta(glm::vec3 delta)
{
  this->at += delta;
  this->eye += delta;
}

void Camera::ensureZoomInRange()
{
  this->zoom = glm::max(0.0001f, this->zoom);
}

void Camera::setZoom(float zoom)
{
  this->zoom = zoom;
  this->ensureZoomInRange();
}

float Camera::getZoom()
{
  return this->zoom;
}

float Camera::increaseZoomBy(float value)
{
  this->zoom += value;
  this->ensureZoomInRange();
}

glm::vec3 Camera::screenToWorldCoordinates(glm::vec2 screenCoordinates)
{
  glm::vec3 pos = this->getPosition();
  pos += this->getFrontUnitVector() * this->zNear;
  pos += this->getRightUnitVector() * ((screenCoordinates.x / this->width) - 0.5) * glm::tan(this->getFovXWithZoom() / 2.0f) * this->zNear * 2.0f;
  pos += this->getDownUnitVector() * ((screenCoordinates.y / this->height) - 0.5) * glm::tan(this->getFovYWithZoom() / 2.0f) * this->zNear * 2.0f;
  return pos;
}
