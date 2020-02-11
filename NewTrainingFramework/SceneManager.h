#pragma once
#include <vector>
#include "Object.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "Light.h"

class SceneManager
{
public:
	
	static SceneManager* GetInstance();
	void Render();
	void Update(float deltaTime);
	bool HasInit();
	void Init();
	void Init(const char* filePath);
	void CleanUp();
	float GetGameTime();

	Light* GetLight(const int &index);

private:
	SceneManager();
	~SceneManager();

	void LoadResource(const char* filePath);
	ObjectType GetType(const char* type);
	std::vector<Object*> m_SceneObjects;
	std::vector<Light*> m_Lights;
	CameraCreationSetting m_CameraCreationSetting;
	glm::vec3 m_AmbientColor;
	float m_AmbientWeight;
	bool m_HasInit;
	float m_GameTime;

	static SceneManager* s_Instance;
};