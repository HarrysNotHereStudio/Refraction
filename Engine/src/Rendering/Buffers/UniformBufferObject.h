#pragma once

#include "BaseBuffer.h"

#include <Math/Matrices.h>

struct sUBO {
	Refraction::Math::Matrix4 viewMatrix;
	Refraction::Math::Matrix4 perspectiveMatrix;
	glm::uint padding;
};

class UniformBufferObject : public BaseBuffer {
public:
	UniformBufferObject(sUBO initData);

	void UploadNewData(sUBO newData);
private:
	sUBO m_currentData;
};