#ifndef POCKET_H
#define POCKET_H
#include <vector>





class Pocket
{

public:
	float radius = 3.5f;
	std::vector<glm::vec3> vertexBufferData;
	std::vector<glm::vec3> colorBufferData;
	float rotation;
	GLuint vertexbuffer;
	GLuint colorbuffer;
	glm::mat4 matrix;
	glm::mat4 MVP;
	glm::vec3 position;
	Pocket(glm::vec3 pocketPosition,float angle);
	~Pocket();
};



#endif