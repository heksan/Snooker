#ifndef TABLE_H
#define TABLE_H






class Table
{

public:
	GLfloat vertex_buffer_data_table[18];
	GLfloat color_buffer_data_table[18];
	GLuint vertexbufferTable;
	GLuint colorbufferTable;
	glm::mat4 matrix;
	glm::mat4 MVP;
	Table();

};



#endif