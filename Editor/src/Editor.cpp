#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <EngineExports.h>
#include <Core/Log.h>
#include <EditorPlatform/OpenGL/ImGuiImpl.h>

namespace REPlatform = RefractionEditor::Platform;

REPlatform::OpenGL::ImGuiImpl ImGuiImpl;
int main() {
	RefractionRuntime::InitWindow();
	auto result = glfwGetCurrentContext();
	ImGuiImpl.Init();

	RefractionRuntime::SetEditorInterfaceDrawCallback([&] {
		ImGuiImpl.Draw();
	});
	RefractionRuntime::InitEngine();

	while (RefractionRuntime::IsRunning()) {
		// do nothing
	}
}
