#include "stdafx.h"
#include "CameraManager.h"

CameraManager* CameraManager::m_Instance = nullptr;

CameraManager* CameraManager::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new CameraManager();
	}
	return m_Instance;
}


void CameraManager::RegisterCamera(const std::string& id, Camera* cam)
{
	m_Cameras.insert(std::make_pair(id, cam));
}

void CameraManager::UnRegisterCamera(const std::string& id)
{
	if (m_Cameras.find(id) != m_Cameras.end() && id != m_ActivedCamera && m_Cameras.size() > 1)
	{
		m_Cameras.erase(id);	
	}
	
}

void CameraManager::SetActiveCamera(const std::string& id)
{
	if (m_Cameras.find(id) != m_Cameras.end())
	{
		m_ActivedCamera = id;
	}
}

Camera* CameraManager::GetActiveCamera()
{
	return m_Cameras.at(m_ActivedCamera);
}

void CameraManager::Update(float deltaTime)
{
	m_Cameras.at(m_ActivedCamera)->Update(deltaTime);
}

Camera* CameraManager::GetCamera(const std::string& id)
{
	if (m_Cameras.find(id) != m_Cameras.end())
	{
		return m_Cameras.at(id);
	}
	return NULL;
}