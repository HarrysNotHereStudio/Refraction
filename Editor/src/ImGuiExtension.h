#pragma once

#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

// Custom ImGui functions
namespace ImGui {
	static void Text(std::string text) { Text(text.c_str()); }
	static bool TreeNode(std::string label) { return TreeNode(label.c_str()); }
}