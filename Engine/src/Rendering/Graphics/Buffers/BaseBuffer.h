#pragma once

#include <GLAD/glad.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <EngineLog.h>
#include <EngineUtilities.h>

class BaseBuffer {
public:
protected:
	GLuint mID;
};