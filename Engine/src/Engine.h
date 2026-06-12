// Functions exposed for Runtime DLL
#pragma once

#include <functional>
#include <string>

namespace RefractionEngine {
	void InitWindow();
	void Init();
	bool IsRunning();
	void Shutdown();
	void SetResourcePath(std::string path);
	void SetViewport(int x, int y, int w, int h);
	void SetEditorInterfaceDrawCallback(std::function<void()> callback);
}