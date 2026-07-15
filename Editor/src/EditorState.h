#pragma once

#include <set>
#include <filesystem>

#include <Core/Common.h>
#include <Classes/Objects/AObject.h>
#include <Math/Rect.h>
#include <Interface/Project.h>

namespace Refraction::Editor {
	struct TempEditorState {
		Common::Ref<Objects::AObject> SelectedObject = nullptr;
		Common::Ref<Engine::Project> ProjectInstance = nullptr;
		bool SimulatingGame = false;
		bool ViewportHovered = false;

		bool PanelViewportVisible = true;
		bool PanelPropertiesVisible = true;
		bool PanelExplorerVisible = true;
		bool PanelStatisticsVisible = true;
		bool PanelLiveCollabVisible = false;
		bool PanelLogVisible = true;
	};
	struct PersistentEditorState {
		std::filesystem::path ExecutableDir = "";
		std::filesystem::path ResourcesDir = "";
		Math::Rect WindowRect = Math::Rect(256, 256, 1280, 720);
		std::set<std::filesystem::path> RecentProjects = {};
	};

	class EditorState {
	public:
		static TempEditorState Temp;
		static PersistentEditorState Persistent;

		static bool Serialise();
		static bool Deserialise();
	};
}
