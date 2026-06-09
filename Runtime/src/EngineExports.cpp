#include <Core/Log.h>
#include <Engine.h>

#include "EngineExports.h"

Exported void TestPrint() {
	Refraction::RuntimeLog::Info("hello world!");
}

Exported void Print(const char* message) {
	Refraction::RuntimeLog::Info(message);
}

Exported void Warn(const char* message) {
	Refraction::RuntimeLog::Warn(message);
}

Exported void Error(const char* message) {
	Refraction::RuntimeLog::Error(message);
}

Exported void SetResourcePath(const char* path) {
	std::string strPath(path);
	Refraction::RuntimeLog::Info("Setting resource path to " + strPath);
	RefractionEngine::SetResourcePath(path);
}

void InitEngine() {
	RefractionEngine::Init();
}

void InitWindow() {
	RefractionEngine::InitWindow();
}