#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include "SceneManager.h"
#include "CameraManager.h"
#include "EffectManager.h"


#define RESOURCE_PATH "../Resources/SM.txt"

SceneManager* SceneManager::s_Instance = NULL;
SceneManager::SceneManager() : m_HasInit(false), m_GameTime(0.0f)
{

}

SceneManager::~SceneManager()
{
	delete s_Instance;
	s_Instance = NULL;
}

SceneManager* SceneManager::GetInstance()
{
	if (NULL == s_Instance)
	{
		s_Instance = new SceneManager();
	}
	return s_Instance;
}

void SceneManager::Init()
{
	LoadResource(RESOURCE_PATH);
	m_HasInit = true;
}

void SceneManager::Init(const char* filePath)
{
	LoadResource(filePath);
	m_HasInit = true;
}

bool SceneManager::HasInit()
{
	return m_HasInit;
}

void SceneManager::Render()
{
	for (auto object : m_SceneObjects)
	{
		object->Render();
	}
	
	for (auto light : m_Lights)
	{
		light->Render();
	}
	
}

void SceneManager::Update(float deltaTime)
{
	m_GameTime += deltaTime;

	for (auto object : m_SceneObjects)
	{
		object->Update(deltaTime);
	}
	for (auto light : m_Lights)
	{
		light->Update(deltaTime);
	}
	CameraManager::GetInstance()->Update(deltaTime);
}

float SceneManager::GetGameTime()
{
	return m_GameTime;
}

void SceneManager::LoadResource(const char* filePath)
{
	std::fstream myFile;
	myFile.open(filePath);
	if (myFile.is_open())
	{

		std::string line;
		
		if (std::getline(myFile, line))
		{
			if (line.find("#Object") != std::string::npos)
			{
				int objectCount;
				
				sscanf(line.c_str(), "#Object: %d", &objectCount);
				for (int index = 0; index < objectCount; index++)
				{
					Object* sceneObject = new Object();
					
					int objectID;
					std::getline(myFile, line);
					if (line.empty())
					{
						std::getline(myFile, line);
					}
					sscanf(line.c_str(), "ID %d", &objectID);
					sceneObject->SetObjectID(objectID);

					int modelID;
					std::getline(myFile, line);				
					sscanf(line.c_str(), "MODEL %d", &modelID);
					sceneObject->SetModel(modelID);

					int textureCount;
					std::getline(myFile, line);
					sscanf(line.c_str(), "TEXTURES %d", &textureCount);
					for (int i = 0; i < textureCount; i++)
					{
						int textureID;
						std::getline(myFile, line);
						sscanf(line.c_str(), "TEXTURE %d", &textureID);
						sceneObject->SetTexture(textureID);
					}

					int cubeTextureCount;
					std::getline(myFile, line);
					sscanf(line.c_str(), "CUBETEXTURES %d", &cubeTextureCount);
					for (int i = 0; i < cubeTextureCount; i++)
					{
						int cubeTextureID;
						std::getline(myFile, line);
						sscanf(line.c_str(), "CUBETEX %d", &cubeTextureID);
						sceneObject->SetCubeTexture(cubeTextureID);
					}

					int shaderID;
					std::getline(myFile, line);
					sscanf(line.c_str(), "SHADER %d", &shaderID);
					sceneObject->SetShader(shaderID);

					int lightCount;
					std::getline(myFile, line);
					sscanf(line.c_str(), "LIGHTS %d", &lightCount);
					for (int i = 0; i < lightCount; i++)
					{
						int lightID;
						std::getline(myFile, line);
						sscanf(line.c_str(), "LIGHT %d", &lightID);
						sceneObject->SetLight(lightID);
					}

					glm::vec3 pos;
					std::getline(myFile, line);
					sscanf(line.c_str(), "POSITION %f, %f, %f", &pos.x, &pos.y, &pos.z);
					sceneObject->SetPosition(pos);

					glm::vec3 rot;
					std::getline(myFile, line);
					sscanf(line.c_str(), "ROTATION %f, %f, %f", &rot.x, &rot.y, &rot.z);
					sceneObject->SetRotation(rot);

					glm::vec3 scale;
					std::getline(myFile, line);
					sscanf(line.c_str(), "SCALE %f, %f, %f", &scale.x, &scale.y, &scale.z);
					sceneObject->SetScale(scale);

					char type[20];
					std::getline(myFile, line);
					sscanf(line.c_str(), "TYPE %s", &type);
					sceneObject->SetType(GetType(type));

					if (std::strcmp(type, "TERRAINS") == 0)
					{
						
						int heightMap;
						std::getline(myFile, line);
						sscanf(line.c_str(), "HEIGHTMAP %d", &heightMap);
						sceneObject->UpdateHeightMap(heightMap);
					}

					m_SceneObjects.push_back(sceneObject);
					
				}
			}
			std::getline(myFile, line);
			std::getline(myFile, line);
			if (line.find("#LIGHTS") != std::string::npos)
			{
				std::getline(myFile, line);
				sscanf(line.c_str(), "AmbientColor %f, %f, %f", &m_AmbientColor.x, &m_AmbientColor.y, &m_AmbientColor.z);
				
				std::getline(myFile, line);
				sscanf(line.c_str(), "AmbientWeight %f", &m_AmbientWeight);

				int lightCount;
				std::getline(myFile, line);
				sscanf(line.c_str(), "LightsCount: %d", &lightCount);

				for (int i = 0; i < lightCount; i++)
				{
					//Light* light = new Light();
					Light* obj = new Light();
					std::getline(myFile, line);
					std::getline(myFile, line);
					std::getline(myFile, line);
					glm::vec3 pos;
					//sscanf(line.c_str(), "POS/DIR %f, %f, %f", &light->pos.x, &light->pos.y, &light->pos.z);
					sscanf(line.c_str(), "POS/DIR %f, %f, %f", &pos.x, &pos.y, &pos.z);
					obj->SetPosition(pos);

					char type[20];
					std::getline(myFile, line);
					sscanf(line.c_str(), "TYPE %s", &type);
					obj->SetLightType(type);

					glm::vec3 color;
					std::getline(myFile, line);
					sscanf(line.c_str(), "COLOR %f, %f, %f", &color.x, &color.y, &color.z);
					obj->SetLightColor(color);

					char moving[20];
					std::getline(myFile, line);
					sscanf(line.c_str(), "MOVING %s", &moving);
					obj->SetLightMoving(moving);

					float radius;
					std::getline(myFile, line);
					sscanf(line.c_str(), "RADIUS %f", &radius);
					obj->SetLightRadius(radius);

					float speed;
					std::getline(myFile, line);
					sscanf(line.c_str(), "SPEED %f", &speed);
					obj->SetLightSpeed(speed);

					obj->SetType(ObjectType::LIGHT);

					obj->SetID(i);

					//m_Light.push_back(light);
					obj->SetModel(3);//hard code use bila model
					obj->SetScale(glm::vec3(0.0005f));
					obj->SetShader(6);
					m_Lights.push_back(obj);

				}
			}
			std::getline(myFile, line);
			std::getline(myFile, line);
			if (line.find("#CAMERA") != std::string::npos)
			{
					std::getline(myFile, line);
					sscanf(line.c_str(), "NEAR %f", &m_CameraCreationSetting.f_near);
					std::getline(myFile, line);
					sscanf(line.c_str(), "FAR %f", &m_CameraCreationSetting.f_far);

					std::getline(myFile, line);
					sscanf(line.c_str(), "FOV %f", &m_CameraCreationSetting.fov);

					std::getline(myFile, line);
					sscanf(line.c_str(), "SPEED %f", &m_CameraCreationSetting.speed);

					//Camera::GetInstance()->Init(m_CameraCreationSetting);
					Camera* fpCam = new Camera();
					fpCam->Init(m_CameraCreationSetting);
					CameraManager::GetInstance()->RegisterCamera("fpCamera", fpCam);
					CameraManager::GetInstance()->SetActiveCamera("fpCamera");
			}
		}
	}
	myFile.close();
}

ObjectType SceneManager::GetType(const char* type)
{
	printf("TYpe=%s\n", type);
	if (strcmp(type, "CHARACTER") == 0)
		return ObjectType::CHARACTER;
	if (strcmp(type, "LIGHT") == 0)
		return ObjectType::LIGHT;
	if (strcmp(type, "TERRAINS") == 0)
		return ObjectType::TERRAINS;
	if (strcmp(type, "REFLECTBALL") == 0)
		return ObjectType::REFLECTBALL;
	if (strcmp(type, "CUBEMAP") == 0)
		return ObjectType::CUBEMAP;
	if (strcmp(type, "FIRE") == 0)
		return ObjectType::FIRE;
	if (strcmp(type, "WATER") == 0)
		return ObjectType::WATER;
	return ObjectType::NONE;
}

Light* SceneManager::GetLight(const int &index)
{
	return m_Lights.at(index);
}

void SceneManager::CleanUp()
{
	for (auto object : m_SceneObjects)
	{
		delete object;
	}

	for (auto light : m_Lights)
	{
		delete light;
	}
}