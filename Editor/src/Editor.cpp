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
			mImGuiImpl = Common::NewRef<Editor::Platform::OpenGL::ImGuiImpl>(mWindow);
			mImGuiImpl->Init();

			mLayerStack->PushLayer(Common::NewRef<EditorLayer>(mLayerStack, mProjectInstance, mWindow, mImGuiImpl));
			//mProjectInstance->New(FileHandling::GetWorkingDirectory() / "TestProject", true);
			mProjectInstance->Open(FileHandling::GetWorkingDirectory() / "TestProject/TestProject.rfp");
		}

	private:
		Common::Ref<Editor::Platform::OpenGL::ImGuiImpl> mImGuiImpl;
	};
}


int main() {
	Refraction::FileHandling::SetResourcesPath("../../Resources");
	try {
		auto instance = Refraction::Editor::EditorInstance();
		instance.Start();
		instance.End();
	} catch (const std::runtime_error& err) {
		Refraction::Log::SError("Error while running EditorInstance: " + std::string(err.what()));
	}
}
