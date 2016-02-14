#ifndef CONTROLS_H
#define CONTROLS_H
#include "ball.h"

void computeCameraMatricesFromInputs();
glm::vec3 computeMovFromInput();
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();

#endif
