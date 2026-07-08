#include <chrono>

#include "Time.h"

using namespace std::chrono;

namespace Refraction::Time {
	double PhysicsDelta = 0;
	double RenderDelta = 0;
	uint64_t SessionStartMSec = (uint64_t)duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	double GetSystemSec() {
		auto duration = system_clock::now().time_since_epoch();
		return (double)duration_cast<seconds>(duration).count();
	}
	uint64_t GetSystemMSec() {
		auto duration = system_clock::now().time_since_epoch();
		return (uint64_t)duration_cast<milliseconds>(duration).count();
	}

	double GetSessionSec() {
		return GetSystemSec() - (SessionStartMSec / static_cast<double>(1000));
	}

	uint64_t GetSessionMSec() {
		return GetSystemMSec() - SessionStartMSec;
	}
}
