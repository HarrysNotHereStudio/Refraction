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
	void BindTextures();
private:
	GLuint mGBuffer = 0;
	GLuint mTextures[GBUFFER_iTEXTURECOUNT];
	GLuint mRBODepth = 0;
};