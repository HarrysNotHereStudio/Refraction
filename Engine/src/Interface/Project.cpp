#include <fstream>

#include <json.hpp>

#include <Core/FileHandling.h>
#include <Classes/Objects/BasicObject.h>
#include <Classes/ClassSerialiser.h>

#include "Project.h"

using nlohmann::json;

namespace Refraction::Engine {
	bool SaveProjectData(const std::filesystem::path& projectFilePath, const ProjectData& projectData) {
		auto pathStr = projectFilePath.string();
		if (!(projectFilePath.has_extension() && projectFilePath.extension() == REFRACTION_PROJECT_EXTENSION)) {
			Log::SError("Invalid project file extension in path " + pathStr);
			return false;
		}
		if (!std::filesystem::exists(projectFilePath.parent_path())) {
			Log::SError("Invalid project directory in path " + pathStr);
			return false;
		}

		json serialised;
		serialised["InitSceneUUID"] = projectData.InitSceneUUID.Serialise();
		Log::SInfo("Saved InitSceneUUID as " + projectData.InitSceneUUID.AsString());
		serialised["Scenes"] = {};
		for (auto& scene : projectData.Scenes) {
			serialised["Scenes"][scene->GetUUID().Serialise()] = Utilities::ClassSerialiser::Serialise(scene);
			Log::SInfo("Serialised scene with UUID " + scene->GetUUID().AsString());
		}
		serialised["GlobalObjects"] = {};
		for (auto& globalObj : projectData.GlobalObjects) {
			serialised["GlobalObjects"][globalObj->GetUUID().Serialise()] = Utilities::ClassSerialiser::Serialise(globalObj);
		}
		std::ofstream dataFile(projectFilePath);
		if (!dataFile.is_open()) {
			Log::SError("Could not open path " + pathStr + " for writing.");
			return false;
		}
		dataFile << serialised;

		Log::SInfo("Saved project data to " + pathStr);
		return true;
	}

	std::optional<ProjectData> LoadProjectData(const std::filesystem::path& projectFilePath) {
		auto pathStr = projectFilePath.string();
		if (!(std::filesystem::exists(projectFilePath) && std::filesystem::is_regular_file(projectFilePath) && projectFilePath.has_extension() && projectFilePath.extension() == REFRACTION_PROJECT_EXTENSION)) {
			Log::SWarn("Invalid or missing project file at path " + pathStr);
			return std::nullopt;
		}

		json data;
		try {
			auto contents = FileHandling::ReadFile(projectFilePath);
			data = json::parse(contents);
			ProjectData deserialised;
			deserialised.InitSceneUUID = UUID::Deserialise(data["InitSceneUUID"]);
			Log::SInfo("Loaded InitSceneUUID as " + deserialised.InitSceneUUID.AsString()); 
			for (auto& sceneData : data.at("Scenes")) {
				auto scene = Utilities::ClassSerialiser::DeserialiseObject<Objects::SceneRoot>(sceneData);
				Log::SInfo("Loaded Scene with UUID " + scene->GetUUID().AsString());
				deserialised.Scenes.push_back(scene);
			}
			for (auto& globalObjData : data.at("GlobalObjects")) {
				auto object = Utilities::ClassSerialiser::DeserialiseObject(globalObjData);
				deserialised.GlobalObjects.push_back(object);
			}

			return std::make_optional(deserialised);
		} catch (const json::parse_error& err) {
			Log::SError("JSON parse error while loading project data at path " + pathStr + ": " + err.what());
			return std::nullopt;
		}
	}

	bool Project::New(const std::filesystem::path& projectPath, bool eraseExisting) {
		auto pathStr = projectPath.string();
		if (!std::filesystem::is_directory(projectPath)) {
			Log::SError(pathStr + " is not a valid directory");
			return false;
		}

		if (std::filesystem::exists(projectPath) && !std::filesystem::is_empty(projectPath)) {
			if (!eraseExisting) {
				Log::SError(pathStr + " already exists and eraseExisting is false");
				return false;
			} else {
				std::filesystem::remove_all(projectPath);
			}
		} else if (!std::filesystem::exists(projectPath)) {
			if (!std::filesystem::create_directory(projectPath)) {
				Log::SError("Failed to create project directory at " + pathStr);
				return false;
			}
		}

		Log::SInfo("Creating project at " + pathStr);

		mProjectPath = projectPath;
		mProjectData = ProjectData{};

		if (!Save()) {
			Log::SError("Failed to create initial save of project data at " + pathStr);
			return false;
		}
		NewScene();
		Log::SInfo("Created project at " + pathStr);
		return true;
	}

	bool Project::Open(const std::filesystem::path& projectFilePath) {
		auto pathStr = projectFilePath.string();
		if (!std::filesystem::exists(projectFilePath) || !std::filesystem::is_regular_file(projectFilePath) || projectFilePath.extension() != REFRACTION_PROJECT_EXTENSION) {
			Log::SError("Attempt to open invalid project path at " + pathStr);
			return false;
		}

		// Close any active project
		if (IsLoaded()) Close();

		auto projectFolderPath = projectFilePath.parent_path();
		mProjectPath = projectFolderPath;

		auto actualProjectFilePath = GetProjectFilePath(projectFolderPath);
		auto projectData = LoadProjectData(actualProjectFilePath);
		mProjectData = projectData.value_or(ProjectData{});

		if (!projectData) Log::SWarn("Failed to load project data at " + pathStr);

		// Open init scene
		if (mProjectData.InitSceneUUID != UUID::Null()) {
			if (!OpenScene(mProjectData.InitSceneUUID)) {
				Log::SWarn("Failed to open specified InitScene, using first scene found instead");
				if (mProjectData.Scenes.size() > 0) {
					mProjectData.InitSceneUUID = mProjectData.Scenes[0]->GetUUID();
					OpenScene(mProjectData.InitSceneUUID);
				}
			}
		}

		Log::SInfo("Opened project at " + pathStr);
		return true;
	}

	bool Project::Save() {
		if (!IsLoaded()) {
			Log::SWarn("Attempt to save project when one isn't loaded");
			return false;
		}

		bool success = true;
		auto projectFilePath = GetProjectFilePath(mProjectPath);
		if (!SaveProjectData(projectFilePath, mProjectData)) {
			success = false;
			Log::SError("Failed to save project data at " + projectFilePath.string());
		} else Log::SInfo("Saved project data at " + projectFilePath.string());

		return success;
	}

	void Project::Close() {
		if (!IsLoaded()) {
			Log::SWarn("Attempt to close project when one isn't loaded");
			return;
		}

		Log::SInfo("Closing project at " + mProjectPath.string());

		for (auto& scene : mProjectData.Scenes) {
			scene.reset();
		}
		for (auto& globalObject : mProjectData.GlobalObjects) {
			globalObject.reset();
		}
		mProjectPath.clear();
		mProjectData = ProjectData{};

		Log::SInfo("Closed project successfully");
	}

	Common::Ref<Objects::SceneRoot> Project::NewScene() {
		Log::SInfo("Creating a new scene");
		auto newScene = Common::NewRef<Objects::SceneRoot>();
		mProjectData.Scenes.push_back(newScene);

		// Instantiate default objects/components
		auto nyenObj = Common::NewRef<Objects::BasicObject>();
		nyenObj->mInstanceName = "Nyen";
		nyenObj->GetComponent<Components::Mesh>()->LoadModel(FileHandling::GetResourcesPath() / "models/nyen/nyen plush.obj");
		nyenObj->GetComponent<Components::APhysics>()->mAngularVelocity = Math::Vector3(0, 64, 0);
		newScene->AddChild(nyenObj);

		auto backpackObj = Common::NewRef<Objects::BasicObject>();
		backpackObj->mInstanceName = "Backpack";
		backpackObj->GetComponent<Components::Mesh>()->LoadModel(FileHandling::GetResourcesPath() / "models/survivalBackpack/backpack.obj");
		backpackObj->mTransform = Math::Transform::FromLookAt(Math::Vector3(0, 14, 10), Math::Vector3::Zero());
		newScene->AddChild(backpackObj);

		Log::SInfo("Successfully created a new scene with UUID " + newScene->GetUUID().AsString());
		mActiveScene = newScene;
		if (mProjectData.InitSceneUUID == UUID::Null()) mProjectData.InitSceneUUID = newScene->GetUUID();
		return mActiveScene;
	}

	bool Project::OpenScene(UUID sceneUUID) {
		Common::Ref<Objects::SceneRoot> targetScene;
		for (auto& scene : mProjectData.Scenes) {
			if (scene->GetUUID().AsInt() == sceneUUID.AsInt()) {
				targetScene = scene;
				break;
			}
		}
		if (!targetScene) {
			Log::SError("Invalid Scene UUID provided (" + sceneUUID.AsString() + ")");
			return false;
		}
		Log::SInfo("Opening scene with UUID " + sceneUUID.AsString());

		mActiveScene = targetScene;
		return true;
	}
}