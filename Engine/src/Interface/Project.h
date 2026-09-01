#pragma once

#include <vector>
#include <filesystem>
#include <optional>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Classes/Objects/AObject.h>
#include <Classes/Objects/SceneRoot.h>
#include <Classes/Objects/Camera.h>
#include <Interface/AssetManager.h>

constexpr auto RFCT_PROJECT_EXTENSION = ".rfp";

namespace Refraction::Engine {

	struct ProjectData {
		UUID InitSceneUUID = UUID::Null();

		std::vector<Common::Shared<Objects::SceneRoot>> Scenes = {};
		std::vector<Common::Shared<Objects::AObject>> GlobalObjects = {};
		Common::Shared<Objects::Camera> ActiveCamera = nullptr;
		bool IsRemote = false; // Determines whether some operations should be ignored as it isn't a local project
	};

	bool SaveProjectData(const std::filesystem::path& projectFilePath, const ProjectData& projectData);

	std::optional<ProjectData> LoadProjectData(const std::filesystem::path& projectFilePath);
	inline std::filesystem::path GetProjectFilePath(const std::filesystem::path& projectPath);

	enum class RemoteProjectCommand {
		AddObject,
		AddComponent,
		UpdateObject,
		UpdateComponent,
		RemoveInstance,
	};

	class Project {
	public:
		static Common::Shared<Project> GetCurrent() { return CurrentProject; }

		// Creates a new project at the given path, returns success
		bool New(const std::filesystem::path& projectPath, bool eraseExisting = false);
		// Initialises as a remote project (live collaboration), returns success
		bool NewRemote();
		// Opens a project at the given path, returns success
		bool Open(const std::filesystem::path& projectPath);

		// Saves the project to disk, returns success
		bool Save();
		// Closes the currently loaded project
		void Close();

		// Handles a message sent by a live collaboration server
		void ProcessRemoteMessage(std::string message);

		// Creates a new scene under the active project, returns the new SceneRoot (empty ptr if failed)
		Common::Shared<Objects::SceneRoot> NewScene();
		// Loads a scene under the active project, returns success
		bool OpenScene(UUID sceneUUID);
		// Returns the currently open scene
		inline Common::Shared<Objects::SceneRoot> GetActiveScene() const { return mActiveScene; }
		inline Common::Shared<AssetManager> GetAssetManager() const { return mAssetManager; }
		// Returns all scenes under this project
		inline std::vector<Common::Shared<Objects::SceneRoot>> GetScenes() const { return mProjectData.Scenes; }
		// Returns all global objects under this project
		inline std::vector<Common::Shared<Objects::AObject>> GetGlobalObjects() const { return mProjectData.GlobalObjects; }
		inline std::filesystem::path GetFilePath() const { return GetProjectFilePath(mProjectPath); }

		inline bool IsLoaded() const { return !mProjectPath.empty(); }
		inline bool IsRemote() const { return mProjectData.IsRemote; }
	private:
		static Common::Shared<Project> CurrentProject;

		static void SetCurrent(Project* instance) {
			CurrentProject = Common::Shared<Project>(instance);
		}

		std::filesystem::path mProjectPath;
		ProjectData mProjectData;

		// Contains all the scenes and global objects
		Common::Shared<Objects::AObject> mRootObject = nullptr;
		Common::Shared<Objects::SceneRoot> mActiveScene = nullptr;
		Common::Shared<Engine::AssetManager> mAssetManager = nullptr;
	};
}

