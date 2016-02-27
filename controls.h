#ifndef CONTROLS_H
#define CONTROLS_H
#include "ball.h"

void computeCameraMatricesFromInputs();
void checkStart(bool& cueStickMoving,float& force);
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();
glm::vec2 castRayThroughMouse();
#endif
