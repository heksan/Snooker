// Include GLEW
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h> 

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "ball.h"

Ball::Ball(int ballID, glm::vec3 position)
{
	id = ballID;
	movementVector = glm::vec3(0, 0, 0);
	ballPosition = position;
	glm::mat4 cueMatrix = glm::mat4(1.0);

	//ids
	//0-white
	//1-15 -red
	//16 yellow
	//17 orange
	//18 green
	//19blue
	//20 pink
	//21black

	if (ballID == 0){
		red = 0.9f;
		green = 0.9f;
		blue = 0.9f;
	}
	if(ballID>0 && ballID<=15){
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	if (ballID == 16){
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	if (ballID == 17){
		red = 1.0f;
		green = 0.7f;
		blue = 0.0f;
	}
	if (ballID == 18){
		red = 0.0f;
		green = 0.8f;
		blue = 0.0f;
	}
	if (ballID == 19){
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
	}
	if (ballID == 20){
		red = 0.9f;
		green = 0.4f;
		blue = 0.7f;
	}
	if (ballID == 21){
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	CreateSphere(2.6f, vertex_buffer_data, color_buffer_data, red, green, blue);
}

Ball::~Ball()
{
}


void CreateSphere(double radius, GLfloat vertex_buffer_data[VertexCount], GLfloat color_buffer_data[VertexCount], float red, float green, float blue) {
	//borrowed and changed from http ://www.swiftless.com/tutorials/opengl/sphere.html

	int n = 0;
	for (double latitude = 0; latitude <= 90 - sides; latitude += sides){
		for (double longtitude = 0; longtitude <= 360 - sides; longtitude += sides){

			//1st quarter
			vertex_buffer_data[n] = float(radius * sin((longtitude) / 180.0f * M_PI) * sin((latitude) / 180.0f * M_PI));
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data[n] = float(radius * cos((longtitude) / 180.0f * M_PI) * sin((latitude) / 180.0f * M_PI));
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data[n] = float(radius * cos((latitude) / 180.0f * M_PI));
			color_buffer_data[n] = blue;
			n++;
			//2nd quarter
			vertex_buffer_data[n] = float(radius * sin((longtitude) / 180.0f * M_PI) * sin((latitude + sides) / 180.0f * M_PI));
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data[n] = float(radius * cos((longtitude) / 180.0f * M_PI) * sin((latitude + sides) / 180.0f * M_PI));
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data[n] =float(radius * cos((latitude + sides) / 180.0f * M_PI));
			color_buffer_data[n] = blue;
			n++;
			//3rd
			vertex_buffer_data[n] = float(radius * sin((longtitude + sides) / 180.0f * M_PI) * sin((latitude) / 180.0f * M_PI));
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data[n] = float(radius * cos((longtitude + sides) / 180.0f * M_PI) * sin((latitude) / 180.0f * M_PI));
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data[n] = float(radius * cos((latitude) / 180.0f * M_PI));
			color_buffer_data[n] = blue;
			n++;
			//4th
			vertex_buffer_data[n] = float(radius * sin((longtitude + sides) / 180.0f * M_PI) * sin((latitude + sides) / 180.0f * M_PI));
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data[n] = float(radius * cos((longtitude + sides) / 180.0f * M_PI) * sin((latitude + sides) / 180.0f * M_PI));
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data[n] = float(radius * cos((latitude + sides) / 180.0f * M_PI));
			color_buffer_data[n] = blue;
			n++;

		}

	}

	//rewrite second half
	for (int m = 0; m < VertexCount / 2; m += 3){
		vertex_buffer_data[m + (VertexCount / 2)] = vertex_buffer_data[m];
		color_buffer_data[m + (VertexCount / 2)] = color_buffer_data[m];
		vertex_buffer_data[m + (VertexCount / 2) + 1] = vertex_buffer_data[m + 1];
		color_buffer_data[m + (VertexCount / 2) + 1] = color_buffer_data[m + 1];
		vertex_buffer_data[m + (VertexCount / 2) + 2] = -vertex_buffer_data[m + 2]; //should be Z = -Z
		color_buffer_data[m + (VertexCount / 2) + 2] = color_buffer_data[m + 2];
	}
}

