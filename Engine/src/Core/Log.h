#pragma once

#include <string>
#include <stdexcept>

namespace Refraction {
	class Log {
	public:
		static std::string GenerateTimestamp();
		static void Info(std::string message);
		static void Warn(std::string message);
		static void Error(std::string message);
	protected:
		static std::string ThreadName;
	};

	class RenderLog : public Log {
	protected:
		static std::string ThreadName;
	};

	class RuntimeLog : public Log {
	protected:
		static std::string ThreadName;
	};

	class PhysicsLog : public Log {
	protected:
		static std::string ThreadName;
	};

}
