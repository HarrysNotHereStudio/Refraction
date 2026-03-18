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

	void BindForWrite();
	void BindForRead();
	void BindAny();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE texType);
	void BindTextures();
private:
	GLuint m_fbo = 0;
	GLuint m_textures[GBUFFER_iTEXTURECOUNT];
	GLuint m_rboDepth = 0;
};