#pragma once

#include <filesystem>
#include <map>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Math/Vector.h>

namespace Refraction::Engine::Platform {
	enum class TextureFormat {
		NONE = 0,
		R8,
		RGB8,
		RGB16F,
		RGB32F,
		RGBA8,
		RGBA16F,
		RGBA32F
	};

	struct TextureStructure {
	public:
		int Width = 1;
		int Height = 1;
		TextureFormat Format = TextureFormat::RGBA8;
		bool MipsEnabled = true;
	};

	class ATexture {
	public:
		// Returns a reference to a new empty texture
		static Common::Ref<ATexture> MakeTexture(const TextureStructure& texStruct);
		// Returns a reference to a texture made with the given path
		static Common::Ref<ATexture> FromPath(std::filesystem::path path);
		// Returns a reference to a texture with the given ID
		static Common::Ref<ATexture> FromID(unsigned int id);

		ATexture() = default;

		// Creates new texture in memory
		virtual void Generate() = 0;
		// Enables read/write on texture
		virtual void Activate(unsigned int unitOffset) = 0;
		virtual void Regenerate(const TextureStructure& texStruct) = 0;
		// Removes from memory
		virtual void Unload() = 0;
		// Returns the width and height of the texture
		virtual Math::Vector2 GetSize() = 0;

		unsigned int GetBufferID() const { return mBufferID; }
	protected:
		static std::map<uint64_t, ATexture*> TexturePool;

		TextureStructure mStructure;
		UUID mUUID;
		unsigned int mBufferID = 0;
	};
}
