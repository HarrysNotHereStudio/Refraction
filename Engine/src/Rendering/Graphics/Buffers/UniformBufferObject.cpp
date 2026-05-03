#include "UniformBufferObject.h"

UniformBufferObject::UniformBufferObject(sUBO initData) {
	m_currentData = initData;
	glGenBuffers(1, &mID);
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sUBO), &initData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mID);
}

void UniformBufferObject::UploadNewData(sUBO newData) {
	m_currentData = newData;
	//Log::Info("New data: x" + std::to_string(m_currentData.viewPosition.x) + " y" + std::to_string(m_currentData.viewPosition.y) + " z" + std::to_string(m_currentData.viewPosition.z));
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sUBO), &m_currentData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}