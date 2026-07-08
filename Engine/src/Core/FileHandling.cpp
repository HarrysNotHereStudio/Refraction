#include <fstream>

#include "FileHandling.h"

// Needs a NULL terminator, otherwise operations like shader compilation will find junk characters at the end of the file
constexpr auto fileTerminator = "\0";

namespace Refraction::FileHandling {
	namespace fs = std::filesystem;

	static fs::path ResourcesPath;

	fs::path GetWorkingDirectory() {
		return fs::current_path();
	}

	fs::path GetResourcesPath() {
		return ResourcesPath;
	}

	void SetResourcesPath(fs::path path) {
		ResourcesPath = path;
	}

	std::string ReadFile(const std::string& filename) {
		if (!fs::exists(filename)) throw std::runtime_error("File " + filename + " does not exist.");
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		};

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		buffer.insert(buffer.end(), *fileTerminator);

		return std::string(buffer.begin(), buffer.end());
	}
	std::string ReadFile(const std::filesystem::path& filePath) {
		return ReadFile(filePath.string());
	}

	bool DoesFileExist(const std::string& path) {
		return fs::exists(path);
	}

	std::vector<fs::directory_entry> GetFilesInFolder(fs::path folderPath) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		std::vector<fs::directory_entry> files;
		for (const auto& file : fs::directory_iterator(folderPath)) {
			files.push_back(file);
		};
		return files;
	};

	std::vector<fs::directory_entry> GetFilesOfExtInFolder(fs::path folderPath, std::string ext) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		std::vector<fs::directory_entry> files;
		for (const auto& file : fs::directory_iterator(folderPath)) {
			if (file.path().extension() == ext) {
				files.push_back(file);
			};
		};
		return files;
	}
	fs::directory_entry GetFirstFileOfExtInFolder(fs::path folderPath, std::string ext) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		for (const auto& file : fs::directory_iterator(folderPath)) {
			if (file.path().extension() == ext) {
				return file;
			};
		};
		return fs::directory_entry();
	};

	std::vector<fs::directory_entry> GetFoldersInFolder(fs::path folderPath) {
		if (!fs::exists(folderPath)) throw std::runtime_error("Path " + folderPath.string() + " does not exist.");
		std::vector<fs::directory_entry> folders;
		for (const auto& folder : fs::recursive_directory_iterator(folderPath)) {
			if (!folder.is_directory()) continue;
			folders.push_back(folder);
		};
		return folders;
	};
}