#include "stdafx.h"
#include "Texture.h"
#include "Globals.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "../Utilities/utilities.h"

Texture::Texture():m_TextureHandle(0),m_Target(0)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureHandle);
}

GLuint Texture::GetTextureHandle()
{
	return m_TextureHandle;
}

GLenum Texture::GetTarget()
{
	return m_Target;
}

void Texture::SetTarget(const char* target)
{
	m_Target = GetEnumFromChar(target);
}

GLenum Texture::GetEnumFromChar(const char* in)
{
	GLenum out = 0;
	std::string str(in);

	if (str.compare("TEXTURE_2D") == 0)
	{
		out = GL_TEXTURE_2D;
	}
	else if (str.compare("TEXTURE_CUBE_MAP") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP;
	}
	else if (str.compare("POSITIVE_X") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	}
	else if (str.compare("NEGATIVE_X") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	}
	else if (str.compare("POSITIVE_Y") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	}
	else if (str.compare("NEGATIVE_Y") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	}
	else if (str.compare("POSITIVE_Z") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	}
	else if (str.compare("NEGATIVE_Z") == 0)
	{
		out = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	}
	else if (str.compare("REPEAT") == 0)
	{
		out = GL_REPEAT;
	}
	else if (str.compare("CLAMP") == 0)
	{
		out = GL_CLAMP_TO_EDGE;
	}

	return out;
}

void Texture::SetTextureID(const int &id)
{
	m_TextureID = id;
}
int Texture::GetTextureID()
{
	return m_TextureID;
}
void Texture::Create2DTextureBuffer(std::string path, const char* tiling)
{
		printf("create tex2d buffer %s\n",path.c_str());
		path.erase(std::remove(path.begin(), path.end(), '"'), path.end());

		int width, height, bpp;
		char* imageData = LoadTGA(path.c_str(), &width, &height, &bpp);

		glGenTextures(1, &m_TextureHandle);
		glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
		if (bpp == 24)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetEnumFromChar(tiling));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetEnumFromChar(tiling));
		
		glBindTexture(GL_TEXTURE_2D, 0);

		SAFE_DEL_ARRAY(imageData);
}
void Texture::CreateCubeTextureBuffer(std::vector<std::string> faces, const char* tiling)
{
	
	glGenTextures(1, &m_TextureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureHandle);
	int width, height, bpp;
	for (int i = 0; i < 6; i++)
	{
		faces[i].erase(std::remove(faces[i].begin(), faces[i].end(), '"'), faces[i].end());
		
		char* faceData = LoadTGA(faces[i].c_str(), &width, &height, &bpp);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetEnumFromChar(tiling));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetEnumFromChar(tiling));
		if (bpp == 24)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, faceData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceData);
		}
		SAFE_DEL_ARRAY(faceData);
	}
}
