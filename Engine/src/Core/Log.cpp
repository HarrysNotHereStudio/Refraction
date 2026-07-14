
#include <format>
#include <chrono>
#include <iostream>
#include <filesystem>

#include <glm/gtc/quaternion.hpp>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/formatting.hpp>

#include "Common.h"


namespace {
	using Refraction::Log;
	std::string ANSI24RGB(Log::Colour colour) {
		return std::format("\033[38;2;{};{};{}m", colour.R, colour.G, colour.B);
	};
	Log::Colour white = { 255,255,255 };
	Log::Colour black = { 0,0,0 };
	Log::Colour separatorColour = white;
	Log::Colour threadColour = white;
	Log::Colour timestampColour = { 64, 210, 255 };
	Log::Colour classColour = { 64, 255, 64 };
	Log::Colour functionColour = { 96, 200, 96 };
	std::string separatorStr = ANSI24RGB(separatorColour) + " - ";
	std::string threadColourStr = ANSI24RGB(threadColour);
	std::string timestampColourStr = ANSI24RGB(timestampColour);
	std::string classColourStr = ANSI24RGB(classColour);
	std::string functionColourStr = ANSI24RGB(functionColour);

	std::string LastClassPrinted = "";
	std::string LastMessagePrinted = "";
}

namespace Refraction {
	void OnConsoleLog(Log::Colour colour, std::string message, bool newLine) {
		using std::vformat, std::make_format_args, std::clog;

		if (newLine) clog << '\n';
		clog << ANSI24RGB(colour) << message << ANSI24RGB(white); // Reset to white after printing
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

	void Log::SInfo(std::string message) {
		GenerateLog("Refraction", message, "INFO", white, Colour{ 200, 255, 255 });
	}
	void Log::SWarn(std::string message) {
		GenerateLog("Refraction", message, "WARN", Colour{ 255, 160, 70 });
	}
	void Log::SError(std::string message) {
		GenerateLog("Refraction", message, "ERR", Colour{ 255, 60, 60 });
	}

	void Log::InitConsoleLog() {
		Log::AddLogCallback(OnConsoleLog);
	}

	void Log::Info(std::string message) {
		GenerateLog(mName, message, "INFO", white, Colour{ 200, 255, 255 });
	}
	void Log::Warn(std::string message) {
		GenerateLog(mName, message, "WARN", Colour{ 255, 160, 70 });
	}
	void Log::Error(std::string message) {
			GenerateLog(mName, message, "ERR", Colour{ 255, 60, 60 });
	}

	void Log::GenerateLog(std::string logName, std::string message, std::string logType, Colour printColour, Colour typeColour) {

		// Get print information
		std::string timestamp = Refraction::Log::GenerateTimestamp();
		auto trace = cpptrace::stacktrace::current();
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

			for (auto& callback : Callbacks) {
				callback(threadColour, logName + " - ", true);
				callback(classColour, "class " + LastClassPrinted, false);
				callback(separatorColour, " - ", false);
			}
		}

		if (typeColour == black) typeColour = printColour;

		// Send to callbacks
		for (auto& callback : Callbacks) {
			callback(timestampColour, "[" + timestamp + "]", true);
			callback(separatorColour, " - ", false);
			callback(typeColour, logType + " ", false);
			callback(functionColour, functionName, false);
			callback(separatorColour, " - ", false);
			callback(printColour, message, false);
		}
	}

	std::vector<Log::LogCallback> Log::Callbacks = {};

	Log Log::Render = Log("Renderer");
	Log Log::Physics = Log("Physics");
	Log Log::Runtime = Log("Runtime");
	Log Log::Editor = Log("Editor");
}
