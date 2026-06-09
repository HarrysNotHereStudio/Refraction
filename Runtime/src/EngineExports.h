#pragma once

#include "Common.h"


extern "C" {
	Exported void TestPrint();

	Exported void Print(const char* message);
	Exported void Warn(const char* message);
	Exported void Error(const char* message);
	
	Exported void SetResourcePath(const char* path);

	Exported void InitEngine();
	Exported void InitWindow();
}
