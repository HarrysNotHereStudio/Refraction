#pragma once

#include <functional>
#include "Common.h"

namespace RefractionRuntime {
	extern "C" {
		Exported void TestPrint();

		Exported void Print(const char* message);
		Exported void Warn(const char* message);
		Exported void Error(const char* message);

		Exported void SetResourcePath(const char* path);
		Exported void SetViewport(int x, int y, int w, int h);
		Exported void SetEditorInterfaceDrawCallback(std::function<void()> callback);

		Exported void InitWindow();
		Exported bool IsRunning();
		Exported void InitEngine();
		Exported void ShutdownEngine();
	}
}
