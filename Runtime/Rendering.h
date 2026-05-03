#pragma once

#include "Common.h"

#include <Log.h>
#include <../Engine.h>

extern "C" {
	Exported int TestFunction() {
		return 67;
	}

	Exported void TestPrint() {
		Log::Info("hello world!");
	}
	
	Exported void SetResourcePath(const char* path) {
		std::string strPath(path);
		Log::Info("Setting resource path to " + strPath);
		RefractionEngine::SetResourcePath(path);
	}

	Exported void InitEngine() {
		RefractionEngine::Init();
	}
}
