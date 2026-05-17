
#include <format>
#include <chrono>
#include <iostream>
#include <filesystem>

#include <glm/gtc/quaternion.hpp>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/formatting.hpp>

#include "EngineLog.h"


namespace {
	std::string ANSI24RGB(int R, int G, int B) {
		return std::format("\033[38;2;{};{};{}m", R, G, B);
	};
	std::string separator = ANSI24RGB(255, 255, 255) + " - ";
	std::string threadColour = ANSI24RGB(255, 255, 255);
	std::string timestampColour = ANSI24RGB(64, 210, 255);
	std::string classColour = ANSI24RGB(64, 255, 64);
	std::string functionColour = ANSI24RGB(96, 200, 96);

	std::string LastClassPrinted = "";
	std::string LastMessagePrinted = "";

	void LogPrint(std::string threadName, std::string message, std::string logType, std::string printColour, std::string typeColour = "") {
		using std::vformat, std::make_format_args, std::clog;

		// Get print information
		std::string timestamp = Log::GenerateTimestamp();
		auto trace = cpptrace::generate_trace();
		// Use the 3rd frame
		auto frameSymbols = cpptrace::prune_symbol(trace.frames[2].symbol);
		std::string fullSymbolStr = frameSymbols;
		std::string functionName = fullSymbolStr;
		std::string className = "???";

		// Get class and function name from the symbols in the frame
		if (auto symbolPos = fullSymbolStr.find_last_of(':'); symbolPos != std::string::npos) {
			functionName = fullSymbolStr.substr(symbolPos + 1, -1);
			auto classSymbolStr = fullSymbolStr.substr(0, symbolPos - 1);

			// Get class name from the remaining symbol string
			if (symbolPos = classSymbolStr.find_last_of(':'); symbolPos != std::string::npos) {
				className = classSymbolStr.substr(symbolPos + 1, -1);
			} else className = classSymbolStr.substr(0, -1);
		}

		// Print class name only once
		if (LastClassPrinted != className) {
			LastClassPrinted = className;
			clog << vformat(threadColour + "{} - " + classColour + "class {}" + separator + "\n", make_format_args(threadName, LastClassPrinted));
		}

		if (typeColour == "") typeColour = printColour;

		// Print message
		LastMessagePrinted = vformat(timestampColour + "[{}]" + separator + typeColour + "{} " + functionColour + "{}" + separator + printColour + "{}\n", make_format_args(timestamp, logType, functionName, message));
		clog << LastMessagePrinted << ANSI24RGB(255, 255, 255); // Reset colour to white after printing
	}
}

std::string Log::GenerateTimestamp() {
	using namespace std::chrono;

	// get time variables
	const auto now = system_clock::now();
	const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
	const auto timer = system_clock::to_time_t(now);

	#pragma warning(suppress : 4996)
	const std::tm bt = *std::localtime(&timer);

	std::ostringstream oss;

	oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
	oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

	return oss.str();
};

void Log::Info(std::string message) {
	LogPrint(ThreadName, message, "INFO", ANSI24RGB(255, 255, 255), ANSI24RGB(200, 255, 255));
}
void Log::Warn(std::string message) {
	LogPrint(ThreadName, message, "WARN", ANSI24RGB(255, 160, 70));
}
void Log::Error(std::string message) {
	LogPrint(ThreadName, message, "ERR", ANSI24RGB(255, 60, 60));
}


std::string Log::ThreadName = "Main";
std::string RenderLog::ThreadName = "Renderer";
std::string RuntimeLog::ThreadName = "Runtime";
std::string PhysicsLog::ThreadName = "Physics";

// Renderer Log

std::string RenderLog::ToString(const glm::vec3 vector) {
	return ("x" + std::to_string(vector.x) + " y" + std::to_string(vector.y) + " z" + std::to_string(vector.z));
}

std::string RenderLog::ToString(const glm::quat quaternion) {
	return ("x" + std::to_string(quaternion.x) + " y" + std::to_string(quaternion.y) + " z" + std::to_string(quaternion.z) + " w" + std::to_string(quaternion.w));
}

std::string RenderLog::ToString(glm::mat4 matrix, const bool pretty) {
	std::string out;
	if (pretty) {
		out += ("x0: " + std::to_string(matrix[0].x) + " y0: " + std::to_string(matrix[0].y) + " z0: " + std::to_string(matrix[0].z) + "\n");
		out += ("x1: " + std::to_string(matrix[1].x) + " y1: " + std::to_string(matrix[1].y) + " z1: " + std::to_string(matrix[1].z) + "\n");
		out += ("x2: " + std::to_string(matrix[2].x) + " y2: " + std::to_string(matrix[2].y) + " z2: " + std::to_string(matrix[2].z) + "\n");
		out += ("x3: " + std::to_string(matrix[3].x) + " y3: " + std::to_string(matrix[3].y) + " z3: " + std::to_string(matrix[3].z) + "\n");
	} else {
		out += ("x0: " + std::to_string(matrix[0].x) + " y0: " + std::to_string(matrix[0].y) + " z0: " + std::to_string(matrix[0].z));
		out += (" x1: " + std::to_string(matrix[1].x) + " y1: " + std::to_string(matrix[1].y) + " z1: " + std::to_string(matrix[1].z));
		out += (" x2: " + std::to_string(matrix[2].x) + " y2: " + std::to_string(matrix[2].y) + " z2: " + std::to_string(matrix[2].z));
		out += (" x3: " + std::to_string(matrix[3].x) + " y3: " + std::to_string(matrix[3].y) + " z3: " + std::to_string(matrix[3].z));
	}
	return out;
}
