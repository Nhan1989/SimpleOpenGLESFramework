#pragma once
#include "glm/glm.hpp"
#include "Model.h"
#include "Shader.h"
#include "Globals.h"
class Light
{
public:
	Light();
	~Light();

	void SetLightType(const char* type);
	void SetLightColor(const glm::vec3 color);
	void SetLightMoving(const char* moving);
	void SetLightRadius(const float radius);
	void SetLightSpeed(const float speed);

	void Render();
	void Update(float deltaTime);

	void SetModel(const int &id);

	void SetShader(const int &id);
	void SetType(ObjectType type);
	void SetPosition(const glm::vec3 &pos);
	glm::vec3 GetPosition();
	void SetRotation(const glm::vec3 &rot);
	void SetScale(const glm::vec3 &scale);

	void UpdateWorldMatrix();
	void SetID(const int &i);
private:
	//use for light object
	char m_LightType[20];
	glm::vec3 m_LightColor;
	char m_LightMoving[20];
	float m_LightRadius;
	float m_LightSpeed;

	glm::vec3 m_Position;
	glm::vec3 m_Rotate;
	glm::vec3 m_Scale;

	Model* m_Model;
	Shader* m_Shader;

	int m_ObjectID;
	ObjectType m_Type;
	
	glm::mat4 m_WorldMatrix;

	float m_angle;
	float m_DeltaTime;
};