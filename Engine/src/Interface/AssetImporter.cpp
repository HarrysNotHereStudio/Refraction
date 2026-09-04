#include <fstream>

#include <Classes/ClassHeaders.h>
#include <Interface/Project.h>

#include "AssetImporter.h"

namespace Refraction::Engine {
	void AssetImporter::Import(std::filesystem::path sourcePath) {
		if (!sourcePath.has_filename()) throw Common::RuntimeError("Cannot import anything other than a file");
		auto currentProject = Project::GetCurrent().lock();
		if (!currentProject) throw Common::RuntimeError("Cannot import without an active project");
		auto importPath = currentProject->GetFilePath().parent_path() / "Assets";

		// Import
		auto dstName = importPath / sourcePath.filename();
		Log::SInfo("Importing file from " + sourcePath.string() + " to " + dstName.string());
		std::filesystem::copy(sourcePath, dstName);
		if (!std::filesystem::exists(dstName)) throw Common::RuntimeError("Failed to copy asset during import");

		// Make metadata file
		Assets::AssetMetadata meta{};
		meta.AssetUUID = UUID::UUID();
		meta.AssetPath = dstName;
		meta.SourcePath = sourcePath;
		meta.FileSize = std::filesystem::file_size(dstName);

		// TODO: determine the AssetType parameter
		auto extension = sourcePath.extension();
		if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
			meta.AssetType = typeid(Assets::Image).name();
		} else if (extension.empty()) {
			auto files = FileHandling::GetFilesInFolder(sourcePath);
			if (files.size() == 2) {
				// Assume it's a shader
				if (FileHandling::GetFirstFileOfExtInFolder(sourcePath, ".frag").exists() && FileHandling::GetFirstFileOfExtInFolder(sourcePath, ".vert").exists()) {
					meta.AssetType = typeid(Assets::Shader).name();
				}
			}
		}
		// Define as regular asset (this prevents crashing but is a problem)
		if (meta.AssetType.empty()) meta.AssetType = typeid(Assets::Asset).name();

		auto metaPath = meta.GetPath(); // GetPath is valid because we set AssetPath
		auto serialised = meta.Serialise();
		std::ofstream dataFile(metaPath);
		if (!dataFile.is_open()) throw Common::RuntimeError(std::format("Could not open {} for writing", metaPath.string()));
		dataFile << serialised;
		dataFile.close();

		// Erase UUID before registering or the deserialisation will cause issues
		meta.AssetUUID.Reset();

		// Register asset
		AssetManager::Try([&](Common::Shared<AssetManager> manager) {
			manager->RegisterAsset(metaPath);
		});
	}
}
