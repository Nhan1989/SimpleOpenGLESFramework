#pragma once
#include "../Utilities/utilities.h"
#include <string>
#include <vector>
class Texture
{
public:
	Texture();
	~Texture();

	void Create2DTextureBuffer(std::string path, const char* tiling);
	void CreateCubeTextureBuffer(std::vector<std::string> face, const char* tiling);

	GLuint GetTextureHandle();

	GLenum GetTarget();
	void SetTarget(const char* target);

	void SetTextureID(const int &id);
	int GetTextureID();

private:
	GLenum GetEnumFromChar(const char* target);

	GLuint m_TextureHandle;
	GLenum m_Target;

	int m_TextureID;
};