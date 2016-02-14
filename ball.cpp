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





Ball::Ball()
{
	id = 10;
	movementVector = glm::vec3(0, 0, 0);
	ballPosition = glm::vec3(0, 0, 0);
}

Ball::Ball(int ballID, glm::vec3 position)
{
	id = ballID;
	movementVector = glm::vec3(0, 0, 0);
	ballPosition = position;
	glm::mat4 cueMatrix = glm::mat4(1.0);
	if (ballID == 0){
		red = 0.9f;
		green = 0.9f;
		blue = 0.9f;
	}
	else{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	std::cout << red;
	CreateSphere(2.6, 0, 0, 0,vertex_buffer_data ,color_buffer_data,red,green,blue );

}

Ball::~Ball()
{
}


void CreateSphere(double R, double H, double K, double Z, GLfloat vertex_buffer_data[VertexCount], GLfloat color_buffer_data[VertexCount], float red, float green, float blue) {



	int n;

	double a;

	double b;



	n = 0;
	for (b = 0; b <= 90 - space; b += space){
		for (a = 0; a <= 360 - space; a += space){
			vertex_buffer_data [n] = R * sin((a) / 180 * M_PI) * sin((b) / 180 * M_PI) - H;
			color_buffer_data [n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a) / 180 * M_PI) * sin((b) / 180 * M_PI) + K;
			color_buffer_data [n] = green;
			n++;
			vertex_buffer_data [n] = R * cos((b) / 180 * M_PI) - Z;
			color_buffer_data [n] = blue;
			n++;



			vertex_buffer_data [n] = R * sin((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI) - H;
			color_buffer_data [n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI) + K;
			color_buffer_data [n] = green;
			n++;
			vertex_buffer_data [n] = R * cos((b + space) / 180 * M_PI) - Z;
			color_buffer_data [n] = blue;
			n++;



			vertex_buffer_data [n] = R * sin((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI) - H;
			color_buffer_data [n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI) + K;
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data [n] = R * cos((b) / 180 * M_PI) - Z;
			color_buffer_data[n] = blue;
			n++;



			vertex_buffer_data [n] = R * sin((a + space) / 180 * M_PI) * sin((b + space) / 180 * M_PI) - H;
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a + space) / 180 * M_PI) * sin((b + space) / 180 * M_PI) + K;
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data [n] = R * cos((b + space) / 180 * M_PI) - Z;
			color_buffer_data[n] = blue;
			n++;



		}

	}
	/*for (int m = 0; m <= 1296; m++){
		vertex_buffer_data [m + 1296] = vertex_buffer_data [1296-m];
		color_buffer_data [m + 1296] = color_buffer_data [1296 - m];
		std::cout << vertex_buffer_data [1296-m];
		m++;
		vertex_buffer_data [m + 1296] = vertex_buffer_data [1296 - m];
		color_buffer_data [m + 1296] = color_buffer_data [1296 - m];
		m++;
		vertex_buffer_data [m + 1296] = -vertex_buffer_data [1296 - m]; //should be Z = -Z
		color_buffer_data [m + 1296] = color_buffer_data [1296 - m];
		

	}*/

	for (b = 0; b <= 90 - space; b += space){
		for (a = 0; a <= 360 - space; a += space){
			vertex_buffer_data [n] = R * sin((a) / 180 * M_PI) * sin((b) / 180 * M_PI) - H;
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a) / 180 * M_PI) * sin((b) / 180 * M_PI) + K;
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data [n] = -R * cos((b) / 180 * M_PI) - Z;
			color_buffer_data[n] = blue;
			n++;



			vertex_buffer_data [n] = R * sin((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI) - H;
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI) + K;
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data [n] = -R * cos((b + space) / 180 * M_PI) - Z;
			color_buffer_data[n] = blue;
			n++;



			vertex_buffer_data [n] = R * sin((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI) - H;
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI) + K;
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data [n] =- R * cos((b) / 180 * M_PI) - Z;
			color_buffer_data[n] = blue;
			n++;



			vertex_buffer_data [n] = R * sin((a + space) / 180 * M_PI) * sin((b + space) / 180 * M_PI) - H;
			color_buffer_data[n] = red;
			n++;
			vertex_buffer_data [n] = R * cos((a + space) / 180 * M_PI) * sin((b + space) / 180 * M_PI) + K;
			color_buffer_data[n] = green;
			n++;
			vertex_buffer_data [n] = -R * cos((b + space) / 180 * M_PI) - Z;
			color_buffer_data[n] = blue;
			n++;



		}

	}


}

