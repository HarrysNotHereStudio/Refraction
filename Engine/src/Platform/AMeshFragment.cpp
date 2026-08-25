#include <Platform/OpenGL/OpenGLMeshFragment.h>
#include <Platform/ARenderingAPI.h>

#include "AMeshFragment.h"

namespace Refraction::Engine::Platform {
	Common::Ref<AMeshFragment> Platform::AMeshFragment::MakeMeshFragment(std::vector<sVertex> vertices, std::vector<unsigned int> indices, Common::Ref<Assets::Material> material) {
		switch (ARenderingAPI::GetAPI()) {
		case RenderingAPI::NONE: default:
			Log::Render.Warn("Attempt to create mesh fragment without an active API");
			return nullptr;
		case RenderingAPI::OPENGL:
			auto fragment = Common::NewRef<OpenGLMeshFragment>(vertices, indices, material);
			fragment->Upload();
			return fragment;
		}
	}

	Platform::AMeshFragment::~AMeshFragment() {}

	AMeshFragment::AMeshFragment(std::vector<sVertex> vertices, std::vector<unsigned int> indices, Common::Ref<Assets::Material> material) {
		mVertices = vertices;
		mIndices = indices;
		mMaterial = material;
	}
}
