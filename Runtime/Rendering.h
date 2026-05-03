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

	Exported void InitEngine() {
		Init();
	}
}
