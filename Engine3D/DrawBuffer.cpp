#pragma once
#include "DrawBuffer.h"
#include "GL/glew.h"
#include <iostream>

DrawBuffer::DrawBuffer()
{
	frameBuffer = 0;
	renderBuffer = 0;
	depthBuffer = 0;
	depthStencilBuffer = 0;
	isStencil = false;
}

DrawBuffer::DrawBuffer(int width, int height, bool stencil, unsigned int texId)
{
	bool fboUsed = true;
	CreateColorBufferAttachment(width, height, texId);

	if (stencil)
		CreateStencilBufferAttachment(width, height, texId + 1);
	else
		CreateDepthBufferAttachment(width, height, texId + 1);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		fboUsed = false;
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	UnBind();
	isStencil = stencil;
}

void DrawBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

}

void DrawBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DrawBuffer::CreateColorBufferAttachment(int width, int height, unsigned int texId)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);

}



void DrawBuffer::CreateDepthBufferAttachment(int width, int height, unsigned int texId)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);


}

void DrawBuffer::CreateStencilBufferAttachment(int width, int height, unsigned int texId)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texId, 0);


}


void DrawBuffer::resize(int width, int height, unsigned int texId)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DrawBuffer::~DrawBuffer(void)
{
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteFramebuffers(1, &renderBuffer);
	if (isStencil)
		glDeleteRenderbuffers(1, &depthStencilBuffer);
	else
		glDeleteRenderbuffers(1, &depthBuffer);
}
