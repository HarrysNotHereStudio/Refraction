// Functions exposed for Runtime DLL
#pragma once

#include <string>

namespace RefractionEngine {
	int Init();
	void InitWindow();
	void SetResourcePath(std::string path);
}