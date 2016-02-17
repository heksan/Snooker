#ifndef CONTROLS_H
#define CONTROLS_H
#include "ball.h"

void computeCameraMatricesFromInputs();
glm::vec3 checkStart();
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();
void castRay();
#endif
