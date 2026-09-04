#include <glad/glad.h>

#include <Settings.h>
#include <Classes/Assets/Shader.h>
#include <Interface/Project.h>

#include "OpenGLGBuffer.h"

namespace Refraction::Engine::Platform {
	OpenGLGBuffer::~OpenGLGBuffer() {
		Cleanup();
	}

	bool OpenGLGBuffer::Init(unsigned int viewWidth, unsigned int viewHeight) {
		// Init framebuffer
		glGenFramebuffers(1, &mFBID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBID);

		auto& graphicsSettings = Settings::CurrentSettings->Graphics;
		auto scale = (graphicsSettings.CFAAEnabled) ? graphicsSettings.CFAAScale : 1;
		int texW = viewWidth * scale;
		int texH = viewHeight * scale;

		TextureStructure texStructDepth = {
			.Width = texW,
			.Height = texH,
			.Format = TextureFormat::NONE,
			.MipsEnabled = true
		};
		auto texStructRGB = texStructDepth;
		texStructRGB.Format = TextureFormat::RGB8;
		auto texStructRGBDisplay = texStructRGB;
		texStructRGBDisplay.Width = viewWidth;
		texStructRGBDisplay.Height = viewHeight;
		auto texStructRGBA16F = texStructDepth;
		texStructRGBA16F.Format = TextureFormat::RGBA16F;

		mDepth = Platform::ATexture::MakeTexture(texStructDepth);
		auto tex = mDepth.lock();
		tex->Activate(0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texW, texH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		mDiffuse = Platform::ATexture::MakeTexture(texStructRGB);
		tex = mDiffuse.lock();
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		mNormal = Platform::ATexture::MakeTexture(texStructRGBA16F);
		tex = mNormal.lock();
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		mPosition = Platform::ATexture::MakeTexture(texStructRGBA16F);
		tex = mPosition.lock();
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		mSMR = Platform::ATexture::MakeTexture(texStructRGB);
		tex = mSMR.lock();
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		mCFAAData = Platform::ATexture::MakeTexture(texStructRGBDisplay);
		tex = mCFAAData.lock();
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		mFinal = Platform::ATexture::MakeTexture(texStructRGBDisplay);
		tex = mFinal.lock();
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		// Frag shader outputs
		unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, attachments); auto result = glGetError();

		// Check status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			if (!status) {
				auto result = glGetError();
				throw std::runtime_error("FRAMEBUFFER CONSTRUCT ERROR | " + result);
			}
			Log::Render.Error("FRAMEBUFFER CONSTRUCT FAILED | " + status);
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

	bool OpenGLGBuffer::Regenerate(unsigned int viewWidth, unsigned int viewHeight) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBID);

		auto& graphicsSettings = Settings::CurrentSettings->Graphics;
		auto scale = (graphicsSettings.CFAAEnabled) ? graphicsSettings.CFAAScale : 1;
		int texW = viewWidth * scale;
		int texH = viewHeight * scale;

		TextureStructure texStructDepth = {
			.Width = texW,
			.Height = texH,
			.Format = TextureFormat::NONE,
			.MipsEnabled = true
		};
		auto texStructRGB = texStructDepth;
		texStructRGB.Format = TextureFormat::RGB8;
		auto texStructRGBDisplay = texStructRGB;
		texStructRGBDisplay.Width = viewWidth;
		texStructRGBDisplay.Height = viewHeight;
		auto texStructRGBA16F = texStructDepth;
		texStructRGBA16F.Format = TextureFormat::RGBA16F;

		auto tex = mDepth.lock();
		tex->Regenerate(texStructDepth);
		tex->Activate(0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texW, texH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		tex = mDiffuse.lock();
		tex->Regenerate(texStructRGB);
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		tex = mNormal.lock();
		tex->Regenerate(texStructRGBA16F);
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		tex = mPosition.lock();
		tex->Regenerate(texStructRGBA16F);
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		tex = mSMR.lock();
		tex->Regenerate(texStructRGB);
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		tex = mCFAAData.lock();
		tex->Regenerate(texStructRGBDisplay);
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		tex = mFinal.lock();
		tex->Regenerate(texStructRGBDisplay);
		tex->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, tex->GetBufferID(), 0);

		// Check status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			Log::Render.Error("FRAMEBUFFER CONSTRUCT ERROR | " + status);
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

	void OpenGLGBuffer::SetShaderTextureIDs() const {
		Common::Ref<Assets::Shader> shaderWeak;
		AssetManager::Try([&](Common::Shared<AssetManager> assetManager) {
			shaderWeak = assetManager->GetAsset<Assets::Shader>("lightingShader");
		});
		if (shaderWeak.expired()) return;
		auto shader = shaderWeak.lock();
		shader->Activate();
		shader->SetUniformInt("gDiffuse", 0);
		shader->SetUniformInt("gNormal", 1);
		shader->SetUniformInt("gPosition", 2);
		shader->SetUniformInt("gSMR", 3);
		shader->SetUniformInt("gDepth", 4);
		shader->SetUniformInt("gCFAAData", 5);
	}

	void OpenGLGBuffer::StartFrame() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBID);
		glDrawBuffer(GL_COLOR_ATTACHMENT5);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLGBuffer::BindFramebufferWrite() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBID);
	}

	void OpenGLGBuffer::BindFramebufferRead() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBID);
	}

	void OpenGLGBuffer::BindFramebufferFull() {
		glBindFramebuffer(GL_FRAMEBUFFER, mFBID);
	}

	void OpenGLGBuffer::BindGeometryPass() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBID);

		GLenum drawBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, drawBuffs);
	}

	void OpenGLGBuffer::BindLightingPass() {
		glDrawBuffer(GL_COLOR_ATTACHMENT5);
		BindTextures();
	}

	void OpenGLGBuffer::BindFinalPass() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBID);
		glReadBuffer(GL_COLOR_ATTACHMENT5);
	}

	void OpenGLGBuffer::BindTextures() {
		auto textures = GetTextureArray();
		for (int i = 0; i < (int)textures.size() - 1; i++) {
			if (textures[i].expired()) continue;
			textures[i].lock()->Activate(i);
		}
	}

	void OpenGLGBuffer::Cleanup() {
		if (mFBID) glDeleteFramebuffers(1, &mFBID);
		auto textures = GetTextureArray();
		for (unsigned int i = 0; i < TextureCount; i++) {
			if (auto tex = textures[i].lock()) tex->Unload();
		}
	}
}
