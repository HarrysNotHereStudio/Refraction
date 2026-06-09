#pragma once

#include <Math/Vector3.h>
#include <Math/Matrices.h>
#include <Math/Transform.h>

#include "BaseModel.h"

class InstancedModel : public BaseModel {
public:
	std::vector<Refraction::Math::Transform> mInstanceTransforms = {};

	InstancedModel(std::string modelSourcePath);

	void AddInstance(Refraction::Math::Vector3 relativePos);
	void DrawModel();
private:
};
