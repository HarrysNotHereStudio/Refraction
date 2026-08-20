#pragma once

#include <Math/Common.h>
#include <Math/Vector.h>

namespace Refraction::Math {
	class Rect {
	public:
		int x, y, w, h;

		// Create a Rect at x,y with the given size
		Rect(int x, int y, int w, int h) : x(x), y(y), w(std::max(w, 0)), h(std::max(h, 0)) {};
		// Create a Rect at x,y with the given size
		Rect(Vector2 pos, int w, int h) : Rect((int)pos.x, (int)pos.y, w, h) {};
		// Create a Rect at x,y with the given size
		Rect(Vector2 pos, Vector2 size) : Rect((int)pos.x, (int)pos.y, (int)size.x, (int)size.y) {};
		// Create a square Rect at x,y with length of n
		Rect(int x, int y, int n) : Rect(x, y, n, n) {};
		// Create a square Rect at x,y with length of n
		Rect(Vector2 pos, int n) : Rect((int)pos.x, (int)pos.y, n, n) {};
		// Create a Rect at 0,0 with the given size
		Rect(int w, int h) : Rect(0, 0, w, h) {};
		// Create a Rect at 0,0 with the given size
		Rect(Vector2 size) : Rect(0, 0, (int)size.x, (int)size.y) {};
		// Create a square Rect at 0,0 with length of n
		Rect(int n) : Rect(0, 0, n, n) {};
		// Create a zero-length Rect at 0,0
		Rect() : x(0), y(0), w(0), h(0) {};

		inline bool operator==(const Rect& other) const {
			return (x == other.x) && (y == other.y) && (w == other.w) && (h == other.h);
		}

		inline float AspectRatio() const { return w / (float)h; }

		inline void ToVector2(Vector2& pos, Vector2& size) const {
			pos.x = (float)x;
			pos.y = (float)y;
			size.x = (float)w;
			size.y = (float)h;
		}

		inline std::string ToString(PrintFormatArgs fmtArgs = PrintFormatArgs()) const {
			if (fmtArgs.Pretty) {
				return std::string("x: " + std::to_string(x) + "\ny: " + std::to_string(y) + "\nw: " + std::to_string(w) + "\nh: " + std::to_string(h));
			} else {
				return std::string("{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + "}");
			}
		}
	};
}
