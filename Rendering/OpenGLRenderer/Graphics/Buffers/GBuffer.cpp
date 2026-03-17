#include "Log.h"

#include "GBuffer.h"

GBuffer::GBuffer() {
}

GBuffer::~GBuffer() {
}

bool GBuffer::Init(unsigned int viewWidth, unsigned int viewHeight) {
	// Init framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	// Init textures
	glGenTextures(GBUFFER_iTEXTURECOUNT, m_textures);

	std::vector<GLenum> drawBuffers = {};
	// Generate RGB textures
	for (unsigned int i = 0; i < GBUFFER_iTEXTURECOUNT; i++) {
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, viewWidth, viewHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(GBUFFER_iTEXTURECOUNT, drawBuffers.data());

	// Depth texture
	glGenRenderbuffers(1, &m_rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewWidth, viewHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

	// Check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Info("FRAMEBUFFER CONSTRUCT ERROR | " + status);
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void GBuffer::BindForWrite() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindForRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindAny() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE texType) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + texType);
}