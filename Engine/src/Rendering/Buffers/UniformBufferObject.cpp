#include "UniformBufferObject.h"

UniformBufferObject::UniformBufferObject(sUBO initData) {
	mCurrentData = initData;
	glGenBuffers(1, &mID);
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sUBO), &mCurrentData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mID);
}

void UniformBufferObject::UploadNewData(sUBO newData) {
	mCurrentData = newData;
	glBindBuffer(GL_UNIFORM_BUFFER, mID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(sUBO), &mCurrentData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}