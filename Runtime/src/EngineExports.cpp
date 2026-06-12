#include <Core/Log.h>
#include <Engine.h>

#include "EngineExports.h"

namespace RefractionRuntime {
	void TestPrint() { Refraction::RuntimeLog::Info("hello world!"); }

	void Print(const char* message) { Refraction::RuntimeLog::Info(message); }
	void Warn(const char* message) { Refraction::RuntimeLog::Warn(message); }
	void Error(const char* message) { Refraction::RuntimeLog::Error(message); }

	void SetResourcePath(const char* path) {
		std::string strPath(path);
		Refraction::RuntimeLog::Info("Setting resource path to " + strPath);
		RefractionEngine::SetResourcePath(path);
	}

	void SetViewport(int x, int y, int w, int h) {
		RefractionEngine::SetViewport(x, y, w, h);
	}

	void SetEditorInterfaceDrawCallback(std::function<void()> callback) {
		RefractionEngine::SetEditorInterfaceDrawCallback(callback);
	}

	void InitWindow() { RefractionEngine::InitWindow(); }
	void InitEngine() { RefractionEngine::Init(); }
	bool IsRunning() { return RefractionEngine::IsRunning(); }
	void ShutdownEngine() { RefractionEngine::Shutdown(); }
}