#include "stdafx.h"
#include "FBO.h"
#include "Globals.h"

FBO::FBO()
{

}

FBO::~FBO()
{
	glDeleteBuffers(1, &m_FBO);
}

GLuint FBO::GetColorBuffer()
{
	return m_ColorBuffer;
}

GLuint FBO::GetFBO()
{
	return m_FBO;
}

GLuint FBO::GetDepthBuffer()
{
	return m_DepthBuffer;
}


void FBO::CreateFBO()
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	//create color texture
	glGenTextures(1, &m_ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::screenWidth, Globals::screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//attach color texture to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//creat depth buffer
	glGenRenderbuffers(1, &m_DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Globals::screenWidth, Globals::screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Error when create frame buffer\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}

void FBO::CreateDepthFBO()
{	
	//Create 2D depth texture
	glGenTextures(1, &m_DepthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_DepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Globals::screenWidth, Globals::screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLint depthBufferBits;
	glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits);
	printf("Depth buffer bits %d\n", depthBufferBits);
	//Gen FBO and attach depth texture to
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Error when create frame buffer line %d - %s", __LINE__, __FILE__);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
}