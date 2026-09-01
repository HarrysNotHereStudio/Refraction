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
		mDepth->Activate(0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texW, texH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth->GetBufferID(), 0);

		mDiffuse = Platform::ATexture::MakeTexture(texStructRGB);
		mDiffuse->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDiffuse->GetBufferID(), 0);

		mNormal = Platform::ATexture::MakeTexture(texStructRGBA16F);
		mNormal->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormal->GetBufferID(), 0);

		mPosition = Platform::ATexture::MakeTexture(texStructRGBA16F);
		mPosition->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mPosition->GetBufferID(), 0);

		mSMR = Platform::ATexture::MakeTexture(texStructRGB);
		mSMR->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mSMR->GetBufferID(), 0);

		mCFAAData = Platform::ATexture::MakeTexture(texStructRGBDisplay);
		mCFAAData->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mCFAAData->GetBufferID(), 0);

		mFinal = Platform::ATexture::MakeTexture(texStructRGBDisplay);
		mFinal->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, mFinal->GetBufferID(), 0);

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

		mDepth->Regenerate(texStructDepth);
		mDepth->Activate(0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texW, texH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth->GetBufferID(), 0);

		mDiffuse->Regenerate(texStructRGB);
		mDiffuse->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDiffuse->GetBufferID(), 0);

		mNormal->Regenerate(texStructRGBA16F);
		mNormal->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormal->GetBufferID(), 0);

		mPosition->Regenerate(texStructRGBA16F);
		mPosition->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mPosition->GetBufferID(), 0);

		mSMR->Regenerate(texStructRGB);
		mSMR->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mSMR->GetBufferID(), 0);

		mCFAAData->Regenerate(texStructRGBDisplay);
		mCFAAData->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mCFAAData->GetBufferID(), 0);

		mFinal->Regenerate(texStructRGBDisplay);
		mFinal->Activate(0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, mFinal->GetBufferID(), 0);

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
		auto assetManager = Project::GetCurrent()->GetAssetManager();
		auto lightingShader = assetManager->GetAsset<Assets::Shader>("lightingShader");
		lightingShader->Activate();
		lightingShader->SetUniformInt("gDiffuse", 0);
		lightingShader->SetUniformInt("gNormal", 1);
		lightingShader->SetUniformInt("gPosition", 2);
		lightingShader->SetUniformInt("gSMR", 3);
		lightingShader->SetUniformInt("gDepth", 4);
		lightingShader->SetUniformInt("gCFAAData", 5);
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
			textures[i]->Activate(i);
		}
	}

	void OpenGLGBuffer::Cleanup() {
		if (mFBID) glDeleteFramebuffers(1, &mFBID);
		auto textures = GetTextureArray();
		for (unsigned int i = 0; i < TextureCount; i++) {
			if (auto& tex = textures[i]) tex->Unload();
		}
	}
}
