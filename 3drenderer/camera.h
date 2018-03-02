#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Camera
{
private:
  // Basic Properties
  glm::vec3 eye;
  glm::vec3 at;
  glm::vec3 up;
  float fovy;
  float zNear;
  float zFar;
  float width;
  float height;

  // Aux Properties
  // Should be in range (0, +inf)
  float zoom;

  // Aux Functions
  float getFovYWithZoom();
  float getFovXWithZoom();
  void ensureZoomInRange();

public:
  Camera( glm::vec3 eye,
          glm::vec3 at,
          glm::vec3 up,
          float fovy,
          float zNear,
          float zFar,
          float width,
          float height);

  // Basic Updates
  void updateWH(float width, float height);

  // Matrices Retrieval
  glm::mat4x4 getViewMatrix();
  glm::mat4x4 getProjectionMatrix();

  // Camera Basic Movement
  glm::vec3 getPosition();
  glm::vec3 getFrontUnitVector();
  glm::vec3 getBackUnitVector();
  glm::vec3 getForwardUnitVector();
  glm::vec3 getBackwardUnitVector();
  glm::vec3 getRightUnitVector();
  glm::vec3 getLeftUnitVector();
  glm::vec3 getUpUnitVector();
  glm::vec3 getDownUnitVector();
  glm::vec3 getAboveUnitVector();
  glm::vec3 getBelowUnitVector();
  void lookAt(glm::vec3 point);
  void lookDelta(glm::vec3 delta);
  void lookRotateHorizontal(float angle);
  void lookRotateVertical(float angle);
  void moveTo(glm::vec3 point);
  void moveDelta(glm::vec3 delta);

  // Camera Mouse Movemnt Aux
   glm::vec3 screenToWorldCoordinates(glm::vec2 screenCoordinates);

  // Zooming
  void setZoom(float zoom);
  float getZoom();
  float increaseZoomBy(float value);

};

#endif // CAMERA_H
