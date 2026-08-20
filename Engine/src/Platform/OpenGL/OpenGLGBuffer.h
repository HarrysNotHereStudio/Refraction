#pragma once

#include <Platform/AGBuffer.h>

namespace Refraction::Engine::Platform {
	class OpenGLGBuffer : public AGBuffer {
	public:
		OpenGLGBuffer() = default;
		~OpenGLGBuffer();
		
		bool Init(unsigned int viewWidth, unsigned int viewHeight) override;
		bool Regenerate(unsigned int viewWidth, unsigned int viewHeight) override;
		void SetShaderTextureIDs() const override;

		void StartFrame() override;
		void BindFramebufferWrite() override;
		void BindFramebufferRead() override;
		void BindFramebufferFull() override;
		void BindGeometryPass() override;
		void BindLightingPass() override;
		void BindFinalPass() override;
		void BindTextures() override;

	protected:
		void Cleanup() override;
	};
}
