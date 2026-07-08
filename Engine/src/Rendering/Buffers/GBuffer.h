#pragma once

#include "BaseBuffer.h"

class GBuffer : public BaseBuffer {
public:
	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_POSITION,
		GBUFFER_TEXTURE_NORMAL,
		GBUFFER_TEXTURE_COLORSPECULAR,
		GBUFFER_iTEXTURECOUNT
	};

	GBuffer();
	~GBuffer();

	bool Init(unsigned int viewWidth, unsigned int viewHeight);
	// Regenerates the GBuffer for a new viewport size
	bool Regenerate(unsigned int viewWidth, unsigned int viewHeight);
	GLuint GetFinalTextureID() const { return mFinalTexture; }

	void BindForWrite();
	void BindForRead();
	void BindFull();
	void BindForLighting();
	void BindForFinal();
	void BindTextures();
private:
	GLuint mTextures[GBUFFER_iTEXTURECOUNT] = {};
	GLuint mRBODepth = 0;
	GLuint mFinalTexture = 0;
};