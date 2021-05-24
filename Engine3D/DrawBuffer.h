#pragma once

class DrawBuffer
{

	unsigned int frameBuffer;
	unsigned int renderBuffer;
	unsigned int depthBuffer;
	unsigned int depthStencilBuffer;
	bool isStencil;

	void CreateDepthBufferAttachment(int width, int height, unsigned int texId);
	void CreateColorBufferAttachment(int width, int height, unsigned int texId);
	void CreateStencilBufferAttachment(int width, int height, unsigned int texId);
public:
	enum
	{
		COLOR,
		DEPTH,
		STENCIL,
		BACK,
		FRONT,
		NONE,
	};
	DrawBuffer();
	DrawBuffer(int width, int height, bool stencil, unsigned int texId);
	void resize(int width, int height, unsigned int texId);
	void Bind();
	void UnBind();
	~DrawBuffer(void);
};
