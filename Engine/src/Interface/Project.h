#pragma once

#include <vector>
#include <filesystem>
#include <optional>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Classes/Objects/AObject.h>
#include <Classes/Objects/SceneRoot.h>
#include <Classes/Objects/Camera.h>

constexpr auto REFRACTION_PROJECT_EXTENSION = ".rfp";

namespace Refraction::Engine {

	struct ProjectData {
		UUID InitSceneUUID = UUID::Null();

		std::vector<Common::Ref<Objects::SceneRoot>> Scenes = {};
		std::vector<Common::Ref<Objects::AObject>> GlobalObjects = {};
		Common::Ref<Objects::Camera> ActiveCamera = nullptr;
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
		Common::Ref<Objects::SceneRoot> NewScene();
		// Loads a scene under the active project, returns success
		bool OpenScene(UUID sceneUUID);
		// Returns the currently open scene
		inline Common::Ref<Objects::SceneRoot> GetActiveScene() const { return mActiveScene; }
		// Returns all scenes under this project
		inline std::vector<Common::Ref<Objects::SceneRoot>> GetScenes() const { return mProjectData.Scenes; }
		// Returns all global objects under this project
		inline std::vector<Common::Ref<Objects::AObject>> GetGlobalObjects() const { return mProjectData.GlobalObjects; }

		inline bool IsLoaded() const { return !mProjectPath.empty(); }
		inline bool IsRemote() const { return mProjectData.IsRemote; }
	private:
		std::filesystem::path mProjectPath;
		ProjectData mProjectData;

		// Contains all the scenes and global objects
		Common::Ref<Objects::AObject> mRootObject = nullptr;
		Common::Ref<Objects::SceneRoot> mActiveScene = nullptr;
	};
}

