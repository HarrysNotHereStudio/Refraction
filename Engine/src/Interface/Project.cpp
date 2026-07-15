#include <fstream>
#include <thread>

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
		serialised["ActiveCameraUUID"] = projectData.ActiveCamera->GetUUID().Serialise();
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

			bool cameraDefined = data.contains("ActiveCameraUUID");
			UUID cameraUUID;
			if (cameraDefined) cameraUUID = UUID::Deserialise(data["ActiveCameraUUID"]);

			for (auto& globalObjData : data.at("GlobalObjects")) {
				auto object = Utilities::ClassSerialiser::DeserialiseObject(globalObjData);
				deserialised.GlobalObjects.push_back(object);
				if (cameraDefined && (object->GetUUID() == cameraUUID)) {
					auto camera = dynamic_pointer_cast<Objects::Camera>(object);
					Objects::Camera::ActiveCamera = camera;
					deserialised.ActiveCamera = camera;
				};
			}

			if (!cameraDefined || !deserialised.ActiveCamera) {
				Log::SWarn("No active camera set, creating new camera");
				auto camera = Common::NewRef<Objects::Camera>();
				Objects::Camera::ActiveCamera = camera;
				deserialised.ActiveCamera = camera;
				deserialised.GlobalObjects.push_back(camera);
			}

			return std::make_optional(deserialised);
		} catch (const json::parse_error& err) {
			Log::SError("JSON parse error while loading project data at path " + pathStr + ": " + err.what());
			return std::nullopt;
		}
	}

	std::filesystem::path GetProjectFilePath(const std::filesystem::path& projectPath) {
		if (!std::filesystem::exists(projectPath)) {
			Log::SError("Project path " + projectPath.string() + " does not exist");
			return std::filesystem::path();
		}
		if (projectPath.empty()) {
			Log::SError("Project path " + projectPath.string() + " is empty");
			return std::filesystem::path();
		}
		std::string projectName = projectPath.filename().string();
		if (projectName.empty()) {
			Log::SError("Could not get project name from path " + projectPath.string());
			return std::filesystem::path();
		}
		return projectPath / (projectName + REFRACTION_PROJECT_EXTENSION);
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

		mRootObject = Common::NewRef<Objects::AObject>();
		mProjectPath = projectPath;
		mProjectData = ProjectData{};

		// Default global objects
		auto camera = Common::NewRef<Objects::Camera>();
		Objects::Camera::ActiveCamera = camera;
		camera->mParent = mRootObject.get();
		mProjectData.ActiveCamera = camera;
		mProjectData.GlobalObjects.push_back(camera);

		if (!Save()) {
			Log::SError("Failed to create initial save of project data at " + pathStr);
			return false;
		}
		NewScene();
		Log::SInfo("Created project at " + pathStr);
		return true;
	}

	bool Project::NewRemote() {
		return false;
	}

	bool Project::Open(const std::filesystem::path& projectFilePath) {
		auto pathStr = projectFilePath.string();
		if (!std::filesystem::exists(projectFilePath) || !std::filesystem::is_regular_file(projectFilePath) || projectFilePath.extension() != REFRACTION_PROJECT_EXTENSION) {
			Log::SError("Attempt to open invalid project path at " + pathStr);
			return false;
		}

		// Close any active project
		if (IsLoaded()) Close();

		// Create new root
		mRootObject = Common::NewRef<Objects::AObject>();

		auto projectFolderPath = projectFilePath.parent_path();
		mProjectPath = projectFolderPath;

		auto actualProjectFilePath = GetProjectFilePath(projectFolderPath);
		auto projectData = LoadProjectData(actualProjectFilePath);
		mProjectData = projectData.value_or(ProjectData{});

		if (!projectData) Log::SWarn("Failed to load project data at " + pathStr);

		for (auto& scene : mProjectData.Scenes) {
			scene->mParent = mRootObject.get();
		}
		for (auto& globalObj : mProjectData.GlobalObjects) {
			globalObj->mParent = mRootObject.get();
		}

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

		Objects::Camera::ActiveCamera = nullptr;
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

	void Project::ProcessRemoteMessage(std::string message) {
		if (!IsRemote()) return; // Not a remote project so this shouldn't run

		Utilities::ClassSerialiser::TryParseJSON(message, [&](nlohmann::json json) {
			if (!json.contains("Command")) {
				Log::Editor.Warn("Unable to process remote message");
				return;
			}
			RemoteProjectCommand cmd = json.at("Command").get<RemoteProjectCommand>();

			switch (cmd) {
			case RemoteProjectCommand::AddObject:
			{
				if (!json.contains("ParentUUID")) {
					Log::Editor.Warn("No Parent UUID provided with command, ignoring message");
					return;
				}
				if (!json.contains("SerialisedObject")) {
					Log::Editor.Warn("No valid serialised data provided with command, ignoring message");
					return;
				}
				auto uuid = UUID::FromExisting(json.at("ParentUUID").get<uint64_t>(), true);
				std::string serialised = json.at("SerialisedObject");

				// Get target with given UUID
				Objects::AObject* parent = nullptr;
				for (auto& obj : mProjectData.GlobalObjects) {
					if (obj->GetUUID() == uuid) {
						parent = obj.get();
						break;
					}
					parent = Objects::AObject::GetInstanceWithUUID(uuid, obj.get());
					if (parent) break;
				}
				if (!parent) {
					for (auto& scene : mProjectData.Scenes) {
						parent = Objects::AObject::GetInstanceWithUUID(uuid, scene.get());
						if (parent) break;
					}
				}

				// Ignore if no target found
				if (!parent) break;

				// Add new object
				auto newObj = Utilities::ClassSerialiser::DeserialiseObject(serialised);
				newObj->mParent = parent;

				Log::Editor.Info("Successfully added object " + uuid.AsString() + " from remote message");
				break;
			}
			case RemoteProjectCommand::AddComponent:
			{
				if (!json.contains("ParentUUID")) {
					Log::Editor.Warn("No Parent UUID provided with command, ignoring message");
					return;
				}
				if (!json.contains("SerialisedComponent")) {
					Log::Editor.Warn("No valid serialised data provided with command, ignoring message");
					return;
				}
				auto uuid = UUID::FromExisting(json.at("ParentUUID").get<uint64_t>(), true);
				std::string serialised = json.at("SerialisedComponent");

				// Get target with given UUID
				Objects::AObject* parent = nullptr;
				for (auto& obj : mProjectData.GlobalObjects) {
					if (obj->GetUUID() == uuid) {
						parent = obj.get();
						break;
					}
					parent = Objects::AObject::GetInstanceWithUUID(uuid, obj.get());
					if (parent) break;
				}
				if (!parent) {
					for (auto& scene : mProjectData.Scenes) {
						parent = Objects::AObject::GetInstanceWithUUID(uuid, scene.get());
						if (parent) break;
					}
				}

				// Ignore if no target found
				if (!parent) break;

				// Add new component
				auto newComp = Utilities::ClassSerialiser::DeserialiseComponent(serialised);
				newComp->mParent = parent;

				Log::Editor.Info("Successfully added component " + uuid.AsString() + " from remote message");
				break;
			}
			case RemoteProjectCommand::UpdateObject:
			{
				if (!json.contains("UUID")) {
					Log::Editor.Warn("No UUID provided with command, ignoring message");
					return;
				}
				if (!json.contains("SerialisedObject")) {
					Log::Editor.Warn("No valid serialised data provided with command, ignoring message");
					return;
				}
				auto uuid = UUID::FromExisting(json.at("UUID").get<uint64_t>(), true);
				std::string serialised = json.at("SerialisedObject");

				// Get target with given UUID
				Objects::AObject* target = nullptr;
				for (auto& obj : mProjectData.GlobalObjects) {
					if (obj->GetUUID() == uuid) {
						target = obj.get();
						break;
					}
					target = Objects::AObject::GetInstanceWithUUID(uuid, obj.get());
					if (target) break;
				}
				if (!target) {
					for (auto& scene : mProjectData.Scenes) {
						target = Objects::AObject::GetInstanceWithUUID(uuid, scene.get());
						if (target) break;
					}
				}

				// Ignore if no target found
				if (!target) break;

				// Update object
				target->Deserialise(serialised);

				Log::Editor.Info("Successfully updated object " + uuid.AsString() + " from remote message");
				break;
			}
			case RemoteProjectCommand::UpdateComponent:
			{
				if (!json.contains("UUID")) {
					Log::Editor.Warn("No UUID provided with command, ignoring message");
					return;
				}
				if (!json.contains("SerialisedComponent")) {
					Log::Editor.Warn("No valid serialised data provided with command, ignoring message");
					return;
				}
				auto uuid = UUID::FromExisting(json.at("UUID").get<uint64_t>(), true);
				std::string serialised = json.at("SerialisedComponent");

				// Get target with given UUID
				Objects::AObject* targetParent = nullptr;
				Components::AComponent* target = nullptr;
				for (auto& obj : mProjectData.GlobalObjects) {
					targetParent = Objects::AObject::GetInstanceWithUUID(uuid, obj.get());
					if (targetParent) break;
				}
				if (!target) {
					for (auto& scene : mProjectData.Scenes) {
						targetParent = Objects::AObject::GetInstanceWithUUID(uuid, scene.get());
						if (targetParent) break;
					}
				}

				// Ignore if no target parent found (therefore component doesn't exist)
				if (!targetParent) break;

				for (auto& comp : *targetParent->GetComponents()) {
					if (comp->GetUUID() != uuid) continue;
					target = comp.get();
					break;
				}

				// Update object
				target->Deserialise(serialised);

				Log::Editor.Info("Successfully updated component " + uuid.AsString() + " from remote message");
				break;
			}
			case RemoteProjectCommand::RemoveInstance:
			{
				if (!json.contains("UUID")) {
					Log::Editor.Warn("No UUID provided with command, ignoring message");
					return;
				}
				auto uuid = UUID::FromExisting(json.at("UUID").get<uint64_t>(), true);

				// Get target with given UUID
				Objects::AObject* target = nullptr;
				for (size_t i = 0; i < mProjectData.GlobalObjects.size(); i++) {
					auto& obj = mProjectData.GlobalObjects[i];
					// Test if this object is the target
					if (obj->GetUUID() == uuid) {
						mProjectData.GlobalObjects.erase(std::next(mProjectData.GlobalObjects.begin(), i - 1));
						break;
					}
					// Test descendants
					target = Objects::AObject::GetInstanceWithUUID(uuid, obj.get());
					if (target) break;
				}

				if (!target) {
					for (size_t i = 0; i < mProjectData.Scenes.size(); i++) {
						auto& obj = mProjectData.Scenes[i];
						// Test if this object is the target
						if (obj->GetUUID() == uuid) {
							mProjectData.Scenes.erase(std::next(mProjectData.Scenes.begin(), i - 1));
							break;
						}
						// Test descendants
						target = Objects::AObject::GetInstanceWithUUID(uuid, obj.get());
						if (target) break;
					}
				}

				// Ignore if no target found
				if (!target) break;

				target->RemoveChild(uuid);

				break;
			}
			}
		});
	}

	Common::Ref<Objects::SceneRoot> Project::NewScene() {
		Log::SInfo("Creating a new scene");
		auto newScene = Common::NewRef<Objects::SceneRoot>();
		newScene->mParent = mRootObject.get();
		mProjectData.Scenes.push_back(newScene);

		// Instantiate default objects/components
		///

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