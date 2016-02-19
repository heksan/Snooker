#ifndef CUESTICK_H
#define CUESTICK_H






class CueStick
{

public:
	GLfloat vertex_buffer_data_stick[18];
	GLfloat color_buffer_data_stick[18];
	GLuint vertexbufferStick;
	GLuint colorbufferStick;
	glm::mat4 matrix;
	glm::mat4 MVP;
	glm::vec3 position;
	glm::vec3 temporaryPosition;
	bool accelerating;
	float rotation;
	float displacement = 85.0f; //displacement of stick centre from cueBall
	CueStick();

};



#endif