#include "stdafx.h"
#include "Globals.h"
#include "Camera.h"
#include "InputManager.h"

const float Camera::CAMERA_MOVE_SPEED = 0.5f;
const float Camera::CAMERA_ROTATE_SPEED = 70.f;
Camera::Camera():m_DeltaTime(0)
{
	
}

Camera::~Camera()
{

}

void Camera::Init(const CameraCreationSetting &setting)
{
	m_CameraPos = glm::vec3(0.0f, 0.1f, 0.5f);
	m_ViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Target = glm::vec3(0.0f, 0.0f, 0.0f);
	m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_DeltaMove = glm::vec3(0.0f);
	m_DeltaRotate = m_ViewDirection;
	m_WorldToViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_ViewDirection, m_CameraUp);

	m_Projector = glm::perspective(glm::radians(setting.fov), (float)(Globals::screenWidth / Globals::screenHeight), setting.f_near, setting.f_far);
}

glm::mat4 Camera::GetProjector()
{
	return m_Projector;
}

glm::vec3 Camera::GetPosition()
{
	return m_CameraPos;
}

void Camera::MoveRight()
{
	m_DeltaMove = glm::normalize(glm::cross(m_ViewDirection, m_CameraUp)) * CAMERA_MOVE_SPEED * m_DeltaTime;
	UpdateWorldToViewMatrix();
}
void Camera::MoveIdle()
{
	m_DeltaMove = glm::vec3(0.0f);
}

void Camera::MoveLeft()
{
	m_DeltaMove = -glm::normalize(glm::cross(m_ViewDirection, m_CameraUp)) * CAMERA_MOVE_SPEED * m_DeltaTime;
	UpdateWorldToViewMatrix();
}

void Camera::MoveUp()
{
	m_DeltaMove = glm::normalize(m_CameraUp) * CAMERA_MOVE_SPEED * m_DeltaTime;
	UpdateWorldToViewMatrix();
}

void Camera::MoveDown()
{
	m_DeltaMove = -glm::normalize(m_CameraUp) * CAMERA_MOVE_SPEED * m_DeltaTime;
	UpdateWorldToViewMatrix();
}

void Camera::MoveForward()
{
	m_DeltaMove = CAMERA_MOVE_SPEED * m_ViewDirection * m_DeltaTime;
	UpdateWorldToViewMatrix();
}

void Camera::MoveBackWard()
{
	m_DeltaMove = -CAMERA_MOVE_SPEED * m_ViewDirection * m_DeltaTime;
	UpdateWorldToViewMatrix();
}

void Camera::TurnLeft()
{
	m_ViewDirection = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-CAMERA_ROTATE_SPEED * m_DeltaTime), m_CameraUp)) *m_ViewDirection;
	UpdateWorldToViewMatrix();
}

void Camera::TurnRight()
{
	m_ViewDirection = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(CAMERA_ROTATE_SPEED * m_DeltaTime), m_CameraUp)) * m_ViewDirection ;
	UpdateWorldToViewMatrix();
}
void Camera::TurnUp()
{
	m_ViewDirection = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(CAMERA_ROTATE_SPEED * m_DeltaTime), glm::cross(m_ViewDirection,m_CameraUp))) * m_ViewDirection;
	
	UpdateWorldToViewMatrix();
}

void Camera::TurnDown()
{

	m_ViewDirection = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-CAMERA_ROTATE_SPEED * m_DeltaTime), glm::cross(m_ViewDirection, m_CameraUp))) * m_ViewDirection;
	
	UpdateWorldToViewMatrix();
}
void Camera::Update(float deltatime)
{
	m_DeltaTime = deltatime;

	unsigned int keyPress = InputManager::GetInstance()->GetKeyPressing();

	if (keyPress & EMove::RIGHT)
	{
		MoveRight();
	}
	if (keyPress & EMove::LEFT)
	{
		MoveLeft();
	}
	if (keyPress & EMove::FORWARD)
	{
		MoveForward();
	}
	if (keyPress & EMove::BACKWARD)
	{
		MoveBackWard();
	}
	if (keyPress & EMove::TURNLEFT)
	{
		TurnLeft();
	}
	if (keyPress & EMove::TURNRIGHT)
	{
		TurnRight();
	}
	if (keyPress & EMove::TURNUP)
	{
		TurnUp();
	}
	if (keyPress & EMove::TURNDOWN)
	{
		TurnDown();
	}
	if (keyPress & EMove::UP)
	{
		MoveUp();
	}
	if (keyPress & EMove::DOWN)
	{
		MoveDown();
	}
}

void Camera::UpdateWorldToViewMatrix()
{
	m_CameraPos += m_DeltaMove;// *deltaTime;
	m_DeltaMove = glm::vec3(0.0f);
	m_WorldToViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_ViewDirection, m_CameraUp);
	glm::vec3 temp = m_CameraPos + m_ViewDirection;
}

glm::mat4 Camera::GetWorldToViewMatrix()
{
	return m_WorldToViewMatrix;
}