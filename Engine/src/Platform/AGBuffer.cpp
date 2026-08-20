#include <Platform/OpenGL/OpenGLGBuffer.h>
#include <Platform/ARenderingAPI.h>

#include "AGBuffer.h"

namespace Refraction::Engine::Platform {
	Common::SRef<AGBuffer> AGBuffer::CreateGBuffer() {
		switch (ARenderingAPI::GetAPI()) {
		case RenderingAPI::NONE: default:
			Log::Render.Warn("Attempt to create GBuffer without an active API");
			return nullptr;
		case RenderingAPI::OPENGL:
			return Common::NewSRef<OpenGLGBuffer>();
		case RenderingAPI::VULKAN:
			Log::Render.Warn("Attempt to create GBuffer with Vulkan. Not implemented yet.");
			return nullptr;
		}
	}

	Common::SRef<Assets::Image> AGBuffer::GetLastRenderedFrame() const {
		auto img = Common::NewSRef<Assets::Image>();
		img->mTexture = mFinal;
		return img;
	}

	AGBuffer::AGBuffer() {}

	AGBuffer::~AGBuffer() {}
}
