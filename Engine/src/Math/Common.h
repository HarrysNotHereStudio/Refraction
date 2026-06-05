#pragma once

namespace Refraction::Math {
	constexpr float PI = 3.14159265358979323846264338327950288f;

	static float ToDegrees(float rad) { return rad * (PI / 180.0f); };
	static float ToRadians(float deg) { return deg * (180.0f / PI); };
	
	template<typename T>
	static inline T Lerp(T start, T end, T alpha) { return start - (start - end) * alpha; };
}
