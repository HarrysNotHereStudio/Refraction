

#include "InstancedModel.h"

namespace RMath = Refraction::Math;

InstancedModel::InstancedModel(std::string modelSourcePath) : BaseModel(modelSourcePath) {
}

void InstancedModel::AddInstance(RMath::Vector3 relativePos) {
	RMath::Transform newTransform = RMath::Transform();
	newTransform.mPosition = relativePos;

	mInstanceTransforms.push_back(newTransform);
}

void InstancedModel::DrawModel() {

	mShader->Activate();
	for (auto& transform : mInstanceTransforms) {
		auto matrix = transform.GetTransform();
		matrix *= mTransform->GetTransform();
		mShader->SetUniformMat4("modelTransform", matrix);

		DrawMeshesRaw();
	}
}
