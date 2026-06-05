#pragma once

#include <GLAD/glad.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class BaseBuffer {
public:
protected:
	GLuint mID;
};