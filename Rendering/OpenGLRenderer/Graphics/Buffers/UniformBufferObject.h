#pragma once

#include "BaseBuffer.h"

struct sUBO {
	glm::mat4 viewMatrix;
	glm::vec3 viewPosition;
	glm::mat4 perspectiveMatrix;
};

class UniformBufferObject : public BaseBuffer {
public:
	UniformBufferObject(sUBO initData);

	void UploadNewData(sUBO newData);
private:
	sUBO m_currentData;
};