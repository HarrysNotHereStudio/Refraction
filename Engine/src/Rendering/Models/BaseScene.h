#pragma once

#include <vector>

#include <EngineClasses/Objects/AObject.h>
#include "PointLight.h"

class BaseScene {
public:
	std::vector<Refraction::Common::Ref<Refraction::Objects::AObject>> mObjects = {};
	std::vector<BaseLight*> mLights = {};
	Refraction::Common::Ref<Refraction::Objects::AObject> mNyen = nullptr;

	BaseScene();
	
	void LoadFromFile(std::string path);
	void Tick(float deltaTime);
	void Render();
private:
	std::string mSourcePath = "";

};
