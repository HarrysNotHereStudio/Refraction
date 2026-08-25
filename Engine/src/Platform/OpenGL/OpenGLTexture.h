#pragma once

#include <Platform/ATexture.h>

namespace Refraction::Engine::Platform {
	class OpenGLTexture : public ATexture {
	public:
		static Common::Ref<ATexture> GetFromPath(std::filesystem::path path);
		static Common::Ref<ATexture> GetFromID(unsigned int id);

		OpenGLTexture(const TextureStructure& texStruct);
		OpenGLTexture(unsigned int id);

		void Generate() override;
		void Activate(unsigned int unitOffset) override;
		void Regenerate(const TextureStructure& texStruct) override;
		void Unload() override;
		Math::Vector2 GetSize() override;
	};
}



