#pragma once
#include "../Utilities/utilities.h"

class FBO
{
public:
	FBO();
	~FBO();

	void CreateDepthFBO();
	void CreateFBO();
	GLuint GetColorBuffer();
	GLuint GetFBO();
	GLuint GetDepthBuffer();

private:
	GLuint m_FBO;
	GLuint m_DepthBuffer;
	GLuint m_ColorBuffer;
};