#include <fstream>

#include <Core/FileHandling.h>
#include <Classes/ClassSerialiser.h>

#include "EditorState.h"

namespace Refraction::Editor {
	TempEditorState EditorState::Temp{};
	PersistentEditorState EditorState::Persistent{};
	
	bool EditorState::Serialise() {
		auto serialised = Utilities::ClassSerialiser::AppendJSON({}, [&](nlohmann::json& json) {
			json["ResourcesDir"] = Persistent.ResourcesDir;
			json["WindowRect"] = Utilities::ClassSerialiser::Serialise(Persistent.WindowRect);
			json["RecentProjects"] = {};
			std::erase_if(Persistent.RecentProjects, [&](std::filesystem::path path) {
				if (!std::filesystem::is_regular_file(path)) return true;
				json["RecentProjects"].push_back(path.string());
				return false;
			});
		});

		auto stateFilePath = Persistent.ExecutableDir / "EditorState.rfc";
		std::ofstream dataFile(stateFilePath);
		if (!dataFile.is_open()) {
			Log::SError("Could not open path " + stateFilePath.string() + " for writing.");
			return false;
		}
		dataFile << serialised.dump(RFCT_JSON_INDENT);
		return true;
	}
	
	bool EditorState::Deserialise() {
		Persistent.ExecutableDir = FileHandling::GetWorkingDirectory();
		auto stateFilePath = Persistent.ExecutableDir / "EditorState.rfc";
		if (!std::filesystem::exists(stateFilePath)) {
			Log::Editor.Warn("No EditorState file found, skipping deserialisation");
			return false;
		}

		auto serialised = FileHandling::ReadFile(stateFilePath);
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			if (json.contains("ResourcesDir")) Persistent.ResourcesDir = std::filesystem::path(json.at("ResourcesDir").get<std::string>());
			if (json.contains("WindowRect")) Persistent.WindowRect = Utilities::ClassSerialiser::DeserialiseRect(json.at("WindowRect"));
			if (json.contains("RecentProjects")) std::for_each(json.at("RecentProjects").begin(), json.at("RecentProjects").end(), [&](std::string path) {
				AddToRecentProjects(path);
			});
		});
		return true;
	}

	void EditorState::AddToRecentProjects(std::filesystem::path projectFilePath) {
		auto& recent = Persistent.RecentProjects;
		for (auto& path : recent) {
			if (projectFilePath == path) {
				recent.erase(std::find(recent.begin(), recent.end(), path));
				break;
			}
		}
		recent.push_back(projectFilePath);
		if (recent.size() > 5) {
			recent.pop_front();
			recent.shrink_to_fit();
		}
	}
}
