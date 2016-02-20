#ifndef POCKET_H
#define POCKET_H






class Pocket
{

public:
	GLfloat vertex_buffer_data[18];
	GLfloat color_buffer_data[18];
	GLuint vertexbuffer;
	GLuint colorbuffer;
	glm::mat4 matrix;
	glm::mat4 MVP;
	glm::vec3 position;
	Pocket(glm::vec3 pocketPosition);
	~Pocket();
};



#endif