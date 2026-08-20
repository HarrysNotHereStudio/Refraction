#include <Platform/Windows/GLFWWindow.h>

#include "AWindow.h"

namespace Refraction::Engine::Platform {
	WindowAPI AWindow::CurrentAPI = WindowAPI::GLFW;

	Common::SRef<AWindow> AWindow::Get() {
		switch (CurrentAPI) {
		case WindowAPI::NONE: default:
			Log::Render.Warn("Skipping window creation, no API defined.");
			return nullptr;
		case WindowAPI::GLFW:
			Log::Render.Info("Creating a Window using GLFW");
			return Common::NewSRef<GLFWWindow>();
		}
	}
}
