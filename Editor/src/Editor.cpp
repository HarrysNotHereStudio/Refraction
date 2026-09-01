#include <iostream>
#include <thread>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Common.h>
#include <Core/Instance.h>

#include <EditorPlatform/OpenGL/ImGuiImpl.h>
#include <EditorLayer.h>


namespace Refraction::Editor {
	class EditorInstance : public Engine::Instance {
	public:
		EditorInstance() : Engine::Instance() {
			mImGuiImpl = Common::NewShared<Editor::Platform::OpenGL::ImGuiImpl>(mWindow);
			mLayerStack->PushLayer(Common::NewShared<EditorLayer>(mLayerStack, mProjectInstance, mWindow, mImGuiImpl));
		}

	private:
		Common::Shared<Editor::Platform::OpenGL::ImGuiImpl> mImGuiImpl;
	};
}


int main() {
	Refraction::FileHandling::SetResourcesPath("./Resources");
	try {
		auto instance = Refraction::Editor::EditorInstance();
		instance.Start();
		instance.End();
	} catch (const std::runtime_error& err) {
		Refraction::Log::SError("Error while running EditorInstance: " + std::string(err.what()));
	}
	auto p = 1;
}
