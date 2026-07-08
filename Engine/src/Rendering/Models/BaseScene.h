#pragma once

#include <vector>

#include <Classes/Objects/AObject.h>
#include "PointLight.h"

class BaseScene {
public:
	std::vector<BaseLight*> mLights = {};

	BaseScene();
private:
};
