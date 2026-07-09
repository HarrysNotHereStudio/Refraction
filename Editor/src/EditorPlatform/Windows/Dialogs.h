#pragma once

#include <filesystem>
#include <string>

namespace Refraction::Editor::Dialogs {
	extern std::filesystem::path SelectFile(const char* ext, const char* title);
	extern std::filesystem::path SelectFolder(const std::string& title = "Select Folder");
}
