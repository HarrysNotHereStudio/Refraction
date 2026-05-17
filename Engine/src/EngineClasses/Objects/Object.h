#pragma once

#include <vector>
#include <string>

#include "../Components/Component.h"

class Object {
public:
	std::string mInstanceName;
protected:
	std::string mDisplayName;
private:
	std::vector<Component*> mComponents;
};
