#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Globals.h"
#include "Light.h"
#include "Heighmap.h"
class Object
{
public:
	Object();
	~Object();

	void SetObjectID(const int &id);
	int GetObjectID();

	void SetModel(const int &id);

	void SetShader(const int &id);

	void SetTexture(const int &id);
	void SetCubeTexture(const int &id);
	void SetLight(const int &id);

	void SetPosition(const glm::vec3 &pos);
	void SetRotation(const glm::vec3 &rot);
	void SetScale(const glm::vec3 &scale);
	void SetType(ObjectType type);

	void UpdateHeightMap(const int &heightMapID);

	void Render();
	void Update(float deltaTime);

private:
	void UpdateWorldMatrix();
	Model* m_Model;
	Shader* m_Shader;
	
	int m_ObjectID;

	std::vector<Texture*> m_Textures;
	Texture* m_CubeTexture;
	std::vector<int> m_LightIDs;

	glm::vec3 m_Position;
	glm::vec3 m_Rotate;
	glm::vec3 m_Scale;
	glm::mat4 m_WorldMatrix;

	ObjectType m_Type;
	float m_angle;

};