#include <Platform/OpenGL/OpenGLGBuffer.h>
#include <Platform/ARenderingAPI.h>
#include <Interface/AssetManager.h>

#include "AGBuffer.h"

namespace Refraction::Engine::Platform {
	Common::Shared<AGBuffer> AGBuffer::CreateGBuffer() {
		switch (ARenderingAPI::GetAPI()) {
		case RenderingAPI::NONE: default:
			Log::Render.Warn("Attempt to create GBuffer without an active API");
			return nullptr;
		case RenderingAPI::OPENGL:
			return Common::NewShared<OpenGLGBuffer>();
		case RenderingAPI::VULKAN:
			Log::Render.Warn("Attempt to create GBuffer with Vulkan. Not implemented yet.");
			return nullptr;
		}
	}

	Common::Ref<Assets::Image> AGBuffer::GetLastRenderedFrame() const {
		Common::Ref<Assets::Image> imgWeak;
		AssetManager::Try([&](Common::Shared<AssetManager> manager) {
			imgWeak = manager->MakeVolatile<Assets::Image>();
		});
		if (imgWeak.expired()) return {};
		auto img = imgWeak.lock();
		img->mTexture = mFinal;
		return img;
	}

	AGBuffer::AGBuffer() {}

	AGBuffer::~AGBuffer() {}
}
