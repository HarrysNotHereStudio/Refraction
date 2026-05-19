#pragma once

#include "Common.h"

#include <EngineLog.h>
#include <Engine.h>

extern "C" {
	Exported int TestFunction() {
		return 67;
	}

	Exported void TestPrint() {
		RuntimeLog::Info("hello world!");
	}

	Exported void Print(const char* message) {
		RuntimeLog::Info(message);
	}
	Exported void Warn(const char* message) {
		RuntimeLog::Warn(message);
	}
	Exported void Error(const char* message) {
		RuntimeLog::Error(message);
	}
	
	Exported void SetResourcePath(const char* path) {
		std::string strPath(path);
		RuntimeLog::Info("Setting resource path to " + strPath);
		RefractionEngine::SetResourcePath(path);
	}

	Exported void InitEngine() {
		RefractionEngine::Init();
	}
}
