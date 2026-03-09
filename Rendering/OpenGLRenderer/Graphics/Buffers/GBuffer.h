#pragma once

#include "BaseBuffer.h"

class GBuffer : public BaseBuffer {
public:
	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_POSITION,
		GBUFFER_TEXTURE_NORMAL,
		GBUFFER_TEXTURE_DIFFUSE,
		GBUFFER_TEXTURE_SMR,
		GBUFFER_iTEXTURECOUNT
	};

	GBuffer();
	~GBuffer();

	bool Init(unsigned int viewWidth, unsigned int viewHeight);

	void StartFrame();
	void BindGeomPass();
	void BindStencilPass();
	void BindLightPass();
	void BindFinalPass();
private:
	GLuint m_fbo;
	GLuint m_textures[GBUFFER_iTEXTURECOUNT];
	GLuint m_depthTexture;
	GLuint m_finalTexture;
};