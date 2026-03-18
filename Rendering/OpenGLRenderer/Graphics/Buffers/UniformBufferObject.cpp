#include "UniformBufferObject.h"

UniformBufferObject::UniformBufferObject(sUBO initData) {
	m_currentData = initData;
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sUBO), &initData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ID);
}

void UniformBufferObject::UploadNewData(sUBO newData) {
	m_currentData = newData;
	//Log::Info("New data: x" + std::to_string(m_currentData.viewPosition.x) + " y" + std::to_string(m_currentData.viewPosition.y) + " z" + std::to_string(m_currentData.viewPosition.z));
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sUBO), &m_currentData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}