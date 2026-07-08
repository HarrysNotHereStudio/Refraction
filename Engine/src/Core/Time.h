#pragma once

#include <cstdint>

namespace Refraction::Time {
	extern double RenderDelta;
	extern double PhysicsDelta;
	extern uint64_t SessionStartMSec;

	// Returns the number of seconds that has passed since unix epoch
	double GetSystemSec();
	// Returns the number of milliseconds that has passed since unix epoch
	uint64_t GetSystemMSec();

	// Returns the number of seconds that has passed since the engine initialised
	double GetSessionSec();
	// Returns the number of milliseconds that has passed since the engine initialised
	uint64_t GetSessionMSec();

	class Timer {
	public:
		// Returns the duration in millseconds
		inline uint64_t GetDurationMSec() const { return mEndMSec - mStartMSec; }
		// Returns the duration in seconds
		inline double GetDurationSec() const { return GetDurationMSec() / static_cast<double>(1000); }
		// Returns true if the timer has finished
		inline bool Completed() const { return GetSessionMSec() >= mEndMSec; }
	private:
		uint64_t mStartMSec;
		uint64_t mEndMSec;
	};
}
