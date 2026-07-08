#include <fstream>

#include <json.hpp>

#include <Core/FileHandling.h>
#include <Classes/ClassSerialiser.h>

#include "EditorState.h"

namespace Refraction::Editor {
	TempEditorState EditorState::Temp{};
	PersistentEditorState EditorState::Persistent{};
	
	bool EditorState::Serialise() {
		using nlohmann::json;
		json serialised;
		serialised["ResourcesDir"] = Persistent.ResourcesDir;
		serialised["WindowRect"] = Utilities::ClassSerialiser::Serialise(Persistent.WindowRect);

		auto stateFilePath = Persistent.ExecutableDir / "EditorState.rfc";
		std::ofstream dataFile(stateFilePath);
		if (!dataFile.is_open()) {
			Log::SError("Could not open path " + stateFilePath.string() + " for writing.");
			return false;
		}
		dataFile << serialised;
		return true;
	}

	bool EditorState::Deserialise() {
		using nlohmann::json;

		Persistent.ExecutableDir = FileHandling::GetWorkingDirectory();
		auto stateFilePath = Persistent.ExecutableDir / "EditorState.rfc";
		if (!std::filesystem::exists(stateFilePath)) {
			Log::Editor.Warn("No EditorState file found, skipping deserialisation");
			return false;
		}

		auto serialised = FileHandling::ReadFile(stateFilePath);
		try {
			json data = json::parse(serialised);
			Persistent.ResourcesDir = std::filesystem::path(data.at("ResourcesDir").get<std::string>());
			Persistent.WindowRect = Utilities::ClassSerialiser::DeserialiseRect(data.at("WindowRect"));
		} catch (const json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON serialised EditorState data: " + std::string(err.what()));
		}
	}
}
