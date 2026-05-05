#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>
#include <format>

namespace fs = std::filesystem;


class Utilities
{
public:
	static std::string EngineWorkingDirectory;

	static int RandomI(const int max, const int min = 0);
	static float RandomF(const float max = 1, const float min = 0);

	static std::vector<char> ReadFile(const std::string& filename);
	static bool ReadFile(const std::string& filename, std::string& buffer); // Writes contents of file to provided string

	static std::vector<fs::directory_entry> GetFilesInFolder(fs::path folderPath);
	static std::vector<fs::directory_entry> GetFilesOfExtInFolder(fs::path folderPath, std::string ext);
	static fs::directory_entry GetFirstFileOfExtInFolder(fs::path folderPath, std::string ext);
	static std::vector<fs::directory_entry> GetFoldersInFolder(fs::path folderPath);

	static std::string CalculateFPS(double deltaTime, int precision = -1);
};