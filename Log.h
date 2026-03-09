#pragma once

#include <string>
#include <format>
#include <tuple>
#include <chrono>
#include <iostream>
#include <filesystem>

using std::string;


class Log {
public:
	static string GenerateTimestamp();
	static void Info(string message);

private:
	static std::string m_lastClassPrinted;
	static std::string m_lastMessagePrinted;
};