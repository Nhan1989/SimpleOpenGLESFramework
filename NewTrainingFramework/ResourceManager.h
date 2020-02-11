#pragma once
#include "Globals.h"
#include <vector>
#include <string>
#include <list>
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "Heighmap.h"
/*
struct Shader
{
	char* FragmentShader;
	char* VertexShader;
	std::vector<std::string> State;
};
*/
class ResourceManager
{
public:
	~ResourceManager();
	ResourceManager();

	static ResourceManager* GetInstance();
	void Init();
	void Init(const char* filePath);
	bool HasInit();
	void LoadResource(const char* file);
	Texture* GetTexture(const int &index);
	Texture* GetCubeTexture(const int &index);
	Model* GetModel(const int &index);
	Shader* GetShader(const int &index);
	Heightmap* ResourceManager::GetHeightMap(const int &index);
	void CleanUp();

private:
	static ResourceManager*		s_Instance;
	char*						m_FilePath;

	std::vector<Model*>			m_Models;
	std::vector<Texture*>		m_Textures;
	std::vector<Texture*>		m_CubeTextures;
	std::vector<Shader*>		m_Shaders;
	std::vector<Heightmap*>		m_HeightMap;


	bool m_HasInit;
};