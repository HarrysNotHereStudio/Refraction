#include "Log.h"

#include "GBuffer.h"

GBuffer::GBuffer() {
}

GBuffer::~GBuffer() {
}

bool GBuffer::Init(unsigned int viewWidth, unsigned int viewHeight) {
	// Init framebuffer
	glGenFramebuffers(1, &mGBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer);

	// Init textures
	glGenTextures(GBUFFER_iTEXTURECOUNT, mTextures);

	// position color buffer
	glBindTexture(GL_TEXTURE_2D, mTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewWidth, viewHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextures[0], 0);
	// normal color buffer
	glBindTexture(GL_TEXTURE_2D, mTextures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewWidth, viewHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mTextures[1], 0);
	// color + specular color buffer
	glBindTexture(GL_TEXTURE_2D, mTextures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewWidth, viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mTextures[2], 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Depth texture
	glGenRenderbuffers(1, &mRBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewWidth, viewHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBODepth);

	// Check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Log::Info("FRAMEBUFFER CONSTRUCT ERROR | " + status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void GBuffer::BindForWrite() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mGBuffer);
}

void GBuffer::BindForRead() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer);
}

void GBuffer::BindAny() {
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
}

void GBuffer::BindTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextures[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mTextures[2]);
}