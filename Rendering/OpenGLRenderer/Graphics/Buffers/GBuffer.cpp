#include "Log.h"

#include "GBuffer.h"

GBuffer::GBuffer() {
}

GBuffer::~GBuffer() {
}

bool GBuffer::Init(unsigned int viewWidth, unsigned int viewHeight) {
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glGenTextures(GBUFFER_iTEXTURECOUNT, m_textures);
	glGenTextures(1, &m_depthTexture);
	glGenTextures(1, &m_finalTexture);

	// Generate RGB textures
	for (unsigned int i = 0; i < GBUFFER_iTEXTURECOUNT; i++) {
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, viewWidth, viewHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	// Depth texture
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, viewWidth, viewHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(GBUFFER_iTEXTURECOUNT, attachments);

	// Check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Info("FRAMEBUFFER CONSTRUCT ERROR | " + status);
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void GBuffer::StartFrame() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::BindGeomPass() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glDrawBuffers(GBUFFER_iTEXTURECOUNT-1, DrawBuffers);
}

void GBuffer::BindStencilPass() {
	// must disable the draw buffers
	glDrawBuffer(GL_NONE);
}

void GBuffer::BindLightPass() {
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	for (unsigned int i = 0; i < GBUFFER_iTEXTURECOUNT; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_POSITION + i]);
	}
}

void GBuffer::BindFinalPass() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT4);
}
