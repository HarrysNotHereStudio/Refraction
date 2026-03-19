#include "InstancedModel.h"

InstancedModel::InstancedModel(std::string modelSourcePath) : BaseModel(modelSourcePath) {
}

void InstancedModel::AddInstance(glm::vec3 relativePos) {
	Transform newTransform = Transform();
	newTransform.position = relativePos;

	mTransforms.push_back(newTransform);
}

void InstancedModel::DrawModel() {

	mShader->Activate();
	for (auto& transform : mTransforms) {
		auto matrix = transform.GetTransform();
		matrix *= mTransform->GetTransform();
		mShader->setUniformMat4("modelTransform", matrix);

		DrawMeshesRaw();
	}
}
