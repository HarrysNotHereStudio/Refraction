#pragma once

#include <filesystem>

#include <Classes/Objects/AObject.h>
#include <Math/Rect.h>
#include <Interface/Project.h>

namespace Refraction::Editor {
	struct TempEditorState {
		Common::Ref<Objects::AObject> SelectedObject = nullptr;
		Common::Ref<Engine::Project> ActiveProject = nullptr;
		bool SimulatingGame = false;
		bool ViewportHovered = false;
	};
	struct PersistentEditorState {
		std::filesystem::path ExecutableDir = "";
		std::filesystem::path ResourcesDir = "";
		Math::Rect WindowRect = Math::Rect(256, 256, 1280, 720);
		std::vector<std::filesystem::path> RecentProjects = {};
	};

	class EditorState {
	public:
		static TempEditorState Temp;
		static PersistentEditorState Persistent;

		static bool Serialise();
		static bool Deserialise();
	};
}
