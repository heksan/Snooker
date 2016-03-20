#ifndef TABLE_H
#define TABLE_H






class Table
{

public:
	GLfloat vertex_buffer_data_table[18];
	//GLfloat vertex_buffer_data_frame[18];
	//GLfloat color_buffer_data_frame[18];
	GLfloat uv_buffer_data_table[12];
	GLuint vertexbufferTable;
	GLuint colorbufferTable;
	GLuint uvbufferTable;
	glm::mat4 matrix;
	glm::mat4 MVP;
	Table();

};



#endif