#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLRenderingAPI.h"

namespace Refraction::Engine::Platform {
	void OpenGLRenderingAPI::Init() {}

	void OpenGLRenderingAPI::Clear(const Math::Vector4 colour) {
		glClearColor(colour.x, colour.y, colour.z, colour.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderingAPI::SetViewportRect(Math::Rect rect) {
		glViewport(rect.x, rect.y, rect.w, rect.h);
	}

}
