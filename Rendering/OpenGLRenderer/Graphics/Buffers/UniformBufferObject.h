#pragma once

#include "BaseBuffer.h"

struct sUBO {
	glm::mat4 viewMatrix;
	glm::mat4 perspectiveMatrix;
	glm::uint padding;
};

class UniformBufferObject : public BaseBuffer {
public:
	UniformBufferObject(sUBO initData);

	void UploadNewData(sUBO newData);
private:
	sUBO m_currentData;
};