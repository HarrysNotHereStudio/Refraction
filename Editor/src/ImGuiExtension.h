#pragma once

#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Math/Vector.h>

// Custom ImGui functions
namespace ImGui {
	static void Text(std::string text) { Text(text.c_str()); }
	static bool TreeNode(std::string label) { return TreeNode(label.c_str()); }
}

namespace Refraction::Math {
	static Vector2 FromImVec2(ImVec2 vec) {
		return Vector2(vec.x, vec.y);
	}
}