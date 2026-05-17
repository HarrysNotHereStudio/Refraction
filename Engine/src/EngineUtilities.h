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

	// Produces a reasonably unique universal ID
	static std::string GenerateUUID();

	// Returns a string of the contents of a file
	static std::string ReadFile(const std::string& filename);

	static std::vector<fs::directory_entry> GetFilesInFolder(fs::path folderPath);
	static std::vector<fs::directory_entry> GetFilesOfExtInFolder(fs::path folderPath, std::string ext);
	static fs::directory_entry GetFirstFileOfExtInFolder(fs::path folderPath, std::string ext);
	static std::vector<fs::directory_entry> GetFoldersInFolder(fs::path folderPath);

	// Converts delta time (in ms) to a rate per second (e.g. FPS)
	static std::string DeltaToRate(double deltaTime, int precision = -1);
};