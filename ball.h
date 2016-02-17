#ifndef BALL_H
#define BALL_H

const int space = 30;
const int VertexCount = (90 / space) * (360 / space) * 4*3*2; //4 quaters 3 - num of positions per vert, 2-halves of ball

class Ball
{
	


public:

	
	float red;
	float green;
	float blue;
	int id;
	int noVertices = VertexCount;
	glm::vec3 deceleration = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 ballPosition;
	GLfloat vertex_buffer_data[VertexCount];
	GLfloat color_buffer_data[VertexCount];
	glm::vec3 movementVector;
	GLuint vertexbuffer;
	GLuint colorbuffer;
	glm::mat4 matrix;
	glm::mat4 MVP;	



	Ball(int id, glm::vec3 ballPosition);
	~Ball();


};


void CreateSphere(double R, double H, double K, double Z, GLfloat vertex_buffer_data[VertexCount], GLfloat color_buffer_data[VertexCount], float red, float green, float blue);


#endif
