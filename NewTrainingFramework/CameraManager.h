#pragma once
#include "Camera.h"
#include <string>
#include <map>

class CameraManager
{
public:
	 static  CameraManager* GetInstance();

	 void RegisterCamera(const std::string& id, Camera* cam);
	 void UnRegisterCamera(const std::string& id);
	 void SetActiveCamera(const std::string& id);
	 void Update(float deltaTime);
	 Camera* GetCamera(const std::string& id);
	 Camera* GetActiveCamera();

private:
	static CameraManager* m_Instance;
	std::map<std::string, Camera*> m_Cameras;
	std::string m_ActivedCamera;

};