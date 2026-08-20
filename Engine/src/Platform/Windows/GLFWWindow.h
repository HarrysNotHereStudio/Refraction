#pragma once

#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <Platform/AWindow.h>

enum WindowInputState {
	VIEWPORT,
	GUI,
	NONE
};

namespace Refraction::Engine::Platform {
	class GLFWWindow : public AWindow {
	public:
		bool mFramebufferResized = false;

		GLFWWindow();

		void Init() override;
		void InitInput() override;
		void OnUpdate(Common::SRef<Objects::Camera> camera) override;
		void Cleanup() override;

		void* GetNativeWindow() const override { return mHandle; }
		bool ShouldClose() const override;
		void SetRect(Math::Rect newRect) override;
	private:
		GLFWwindow* mHandle = nullptr;
	};
}
