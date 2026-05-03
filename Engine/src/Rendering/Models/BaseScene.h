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

	BaseScene(std::string resourcesPath);
	
	void LoadFromFile(std::string path);
	void Tick(float deltaTime);
private:
	std::string mSourcePath = "";

	BaseModel* mNyen = nullptr;
};
