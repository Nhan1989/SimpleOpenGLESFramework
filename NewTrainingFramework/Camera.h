#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Globals.h"
class Camera
{
public:

	void Init(const CameraCreationSetting &setting);
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
	void MoveIdle();
	void MoveForward();
	void MoveBackWard();
	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();

	void Update(float deltaTime);
	
	glm::mat4 GetWorldToViewMatrix();
	glm::mat4 GetProjector();
	glm::vec3 GetPosition();
	
	Camera();
	~Camera();

private:
	
	void UpdateWorldToViewMatrix();
	glm::mat4 m_WorldToViewMatrix;
	glm::vec3 m_CameraPos;
	glm::vec3 m_Target;
	glm::vec3 m_CameraUp;
	glm::vec3 m_ViewDirection;
	glm::vec3 m_DeltaMove;
	glm::vec3 m_DeltaRotate;
	float m_DeltaTime;
	glm::mat4 m_Projector;

	const static float CAMERA_MOVE_SPEED;
	const static float CAMERA_ROTATE_SPEED;
	

};