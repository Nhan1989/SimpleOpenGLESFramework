#include "stdafx.h"
#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include "Vertex.h"
#define RESOURCE_PATH "../Resources/RM.txt"


ResourceManager* ResourceManager::s_Instance = NULL;

ResourceManager::ResourceManager():m_HasInit(false)
{

}

ResourceManager::~ResourceManager()
{
	delete s_Instance;
	s_Instance = NULL;
}

void ResourceManager::Init()
{
	LoadResource(RESOURCE_PATH);
	m_HasInit = true;
}

void ResourceManager::Init(const char* filePath)
{
	LoadResource(filePath);
	m_HasInit = true;
}

ResourceManager* ResourceManager::GetInstance()
{
	if (NULL == s_Instance)
	{
		s_Instance = new ResourceManager();
	}
	return s_Instance;
}

bool ResourceManager::HasInit()
{
	return m_HasInit;
}

Texture* ResourceManager::GetTexture(const int &index)
{
	if (index > m_Textures.size() || index < 0)
	{
		esLogMessage("Invail Texture ID %d", index);
		return nullptr;
	}
	return m_Textures.at(index);
}

Texture* ResourceManager::GetCubeTexture(const int &index)
{
	if (index > m_CubeTextures.size() || index < 0)
	{
		esLogMessage("Invail Cube Texture ID %d", index);
		return nullptr;
	}
	return m_CubeTextures.at(index);
}

Heightmap* ResourceManager::GetHeightMap(const int &index)
{
	if (index > m_HeightMap.size() || index < 0)
	{
		esLogMessage("Invail Cube Texture ID %d", index);
		return nullptr;
	}
	return m_HeightMap.at(index);
}

Model* ResourceManager::GetModel(const int &index)
{
	if (index > m_Models.size() || index < 0)
	{
		esLogMessage("Invail Model ID %d", index);
		return nullptr;
	}
	return m_Models.at(index);
}

Shader* ResourceManager::GetShader(const int &index)
{
	if (index > m_Shaders.size() || index < 0)
	{
		esLogMessage("Invail Shader ID %d", index);
		return nullptr;
	}
	return m_Shaders.at(index);
}

void ResourceManager::LoadResource(const char* filePath)
{
	std::fstream myFile;
	myFile.open(filePath);
	
	if (myFile.is_open())
	{
		printf("filePath %s\n", filePath);
		
		std::string line;
		while (std::getline(myFile, line))
		{
			if (line.empty())
				continue;

			if (line.find("#Models:") != std::string::npos)
			{
				int modelCount;
				sscanf(line.c_str(), "#Models: %d", &modelCount);
				for (int i = 0; i < modelCount; i++)
				{
					char file[100];
					std::getline(myFile, line);
					std::getline(myFile, line);
					sscanf(line.c_str(), "FILE %s", &file);
					Model* model = new Model(file);
					m_Models.push_back(model);
				}
			}

			if (line.find("#2D Texture:") != std::string::npos)
			{
				int textureCount;
				sscanf(line.c_str(), "#2D Texture: %d", &textureCount);
				for (int i = 0; i < textureCount; i++)
				{
					char file[100];
					char tiling[20];
					int id;
					Texture* tex = new Texture();
					tex->SetTarget("TEXTURE_2D");
					std::getline(myFile, line);
					sscanf(line.c_str(), "ID %d", &id);
					tex->SetTextureID(id);
					std::getline(myFile, line);
					sscanf(line.c_str(), "FILE %s", &file);
					std::getline(myFile, line);
					sscanf(line.c_str(), "TILING %s", &tiling);
					tex->Create2DTextureBuffer(std::string(file), tiling);
					m_Textures.push_back(tex);
				}
			}

			if (line.find("#Cube Texture:") != std::string::npos)
			{
				int cubeTextureCount;
				sscanf(line.c_str(), "#Cube Texture: %d", &cubeTextureCount);
				for (int i = 0; i < cubeTextureCount; i++)
				{
					std::vector<std::string> cubeFaces;
					int id;
					char tiling[20];
					Texture* tex = new Texture();
					tex->SetTarget("TEXTURE_CUBE_MAP");
					std::getline(myFile, line);
					sscanf(line.c_str(), "ID %d", &id);
					tex->SetTextureID(id);
					for (int j = 0; j < 6; j++)
					{
						char face[100];
						std::getline(myFile, line);
						sscanf(line.c_str(), "FILE %s", &face);
						cubeFaces.push_back(std::string(face));
					}
					std::getline(myFile, line);
					sscanf(line.c_str(), "TILING %s", &tiling);
					tex->CreateCubeTextureBuffer(cubeFaces, tiling);
					//m_Textures.push_back(tex);
					m_CubeTextures.push_back(tex);
				}
			}

			if (line.find("#Shaders:") != std::string::npos)
			{
				int shaderCount;
				sscanf(line.c_str(), "#Shaders: %d", &shaderCount);
				for (int i = 0; i < shaderCount; i++)
				{
					char vs[100];
					char fs[100];
					Shader* shader = new Shader();
					std::getline(myFile, line);
					std::getline(myFile, line);
					sscanf(line.c_str(), "VS %s", &vs);

					std::getline(myFile, line);
					sscanf(line.c_str(), "FS %s", &fs);

					int stateCount;
					std::getline(myFile, line);
					sscanf(line.c_str(), "STATES %d", &stateCount);
					for (int k = 0; k < stateCount; k++)
					{
						char state[100];
						std::getline(myFile, line);
						sscanf(line.c_str(), "STATE %s", &state);
						shader->SetState(state);
					}

					shader->Init(vs, fs);
					m_Shaders.push_back(shader);
					std::getline(myFile, line);
				}

			}

			if (line.find("#Height Map:") != std::string::npos)
			{
				int count;
				sscanf(line.c_str(), "#Height Map: %d", &count);
				
				for (int i = 0; i < count; i++)
				{
					char file[100];
					Heightmap* hMap = new Heightmap();
					std::getline(myFile, line);
					std::getline(myFile, line);
					sscanf(line.c_str(), "FILE %s", &file);
					hMap->loadRAW(128,128,file,0.05,0.0);
					m_HeightMap.push_back(hMap);
				}
			}
		}
	}
	myFile.close();
}

void ResourceManager::CleanUp()
{	

 	for (auto model:m_Models)
	{
		delete model;
	}

	for (auto texture:m_Textures)
	{
		delete texture;
	}

	for (auto texture:m_CubeTextures)
	{
		delete texture;
	}

	for (auto shader:m_Shaders)
	{
		delete shader;
	}
	
}