// C++/CLR wrapper to allow C# to communicate with the engine

#include "Common.h"

#include "Rendering.h"

#pragma unmanaged

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			std::cout << "Process attached to Refraction Runtime\n";
			break;
		case DLL_PROCESS_DETACH:
			std::cout << "Process detached from Refraction Runtime\n";
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}
