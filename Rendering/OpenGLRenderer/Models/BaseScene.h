#pragma once

#include <vector>

#include "../ShaderManager.h"
#include "BaseModel.h"
#include "InstancedModel.h"
#include "PointLight.h"

class BaseScene {
public:
	std::vector<BaseModel*> mModels = {};
	std::vector<BaseLight*> mLights = {};

	BaseScene();
	
	void LoadFromFile(std::string path);
private:
	std::string mSourcePath = "";
};
