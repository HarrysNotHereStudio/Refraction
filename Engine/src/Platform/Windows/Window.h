#pragma once

#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Platform/AWindow.h>
#include <Platform/OpenGL/ImGuiImpl.h>
#include <Rendering/Models/BaseCamera.h>

enum WindowInputState {
	VIEWPORT,
	GUI,
	NONE
};

namespace Refraction::Platform::Windows {
	class Window : public AWindow {
	public:
		typedef Refraction::Platform::OpenGL::ImGuiImpl ImGuiImpl;
		Refraction::Common::URef<ImGuiImpl> mImGuiImpl = nullptr;

		bool mFramebufferResized = false;

		Window();

		int Init() override;
		void InitInput() override;
		void OnUpdate() override;
		void Cleanup() override;

		void* GetNativeWindow() const override { return mWindow; }
		BaseCamera* GetCurrentCamera() { return mCurrentCamera; }
		void SetCurrentCamera(BaseCamera* pNewCamera) { mCurrentCamera = pNewCamera; }
	private:
		BaseCamera* mCurrentCamera = nullptr;
		GLFWwindow* mWindow = nullptr;
	};
}
