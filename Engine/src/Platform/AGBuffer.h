#pragma once

#include <Core/Common.h>
#include <Classes/Assets/Image.h>
#include <Platform/ATexture.h>

namespace Refraction::Engine::Platform {
	class AGBuffer {
	public:
		// Creates a new GBuffer using the active rendering API
		static Common::Shared<AGBuffer> CreateGBuffer();

		Common::Shared<Assets::Image> GetLastRenderedFrame() const;

		// Initialises the GBuffer textures
		virtual bool Init(unsigned int viewWidth, unsigned int viewHeight) = 0;
		// Regenerates the GBuffer for a new viewport size
		virtual bool Regenerate(unsigned int viewWidth, unsigned int viewHeight) = 0;
		virtual void SetShaderTextureIDs() const = 0;

		virtual void StartFrame() = 0;
		virtual void BindFramebufferWrite() = 0;
		virtual void BindFramebufferRead() = 0;
		virtual void BindFramebufferFull() = 0;
		virtual void BindGeometryPass() = 0;
		virtual void BindLightingPass() = 0;
		virtual void BindFinalPass() = 0;
		virtual void BindTextures() = 0;

	protected:
		static const unsigned int TextureCount = 6;

		unsigned int mFBID = 0;
		Common::Shared<Platform::ATexture> mDiffuse;
		Common::Shared<Platform::ATexture> mNormal;
		Common::Shared<Platform::ATexture> mPosition;
		Common::Shared<Platform::ATexture> mSMR; // Specular R, Metallic G, Roughness B
		Common::Shared<Platform::ATexture> mDepth;
		Common::Shared<Platform::ATexture> mCFAAData;
		Common::Shared<Platform::ATexture> mFinal;

		AGBuffer();
		virtual ~AGBuffer();

		inline std::vector<Common::Shared<Platform::ATexture>> GetTextureArray() {
			return { mDiffuse, mNormal, mPosition, mSMR, mDepth, mCFAAData, mFinal };
		};

		virtual void Cleanup() = 0;
	};
}
