#include "stdafx.h"
#include "Light.h"
#include "ResourceManager.h"
#include "CameraManager.h"

Light::Light():m_angle(0.0f)
{

}

Light::~Light()
{

}

void Light::SetModel(const int &id)
{
	m_Model = ResourceManager::GetInstance()->GetModel(id);
}

void Light::SetShader(const int &id)
{
	m_Shader = ResourceManager::GetInstance()->GetShader(id);
}

void Light::SetType(ObjectType type)
{
	m_Type = type;
}

void Light::SetPosition(const glm::vec3 &pos)
{
	m_Position = pos;
}

glm::vec3 Light::GetPosition()
{
	return m_Position;
}

void Light::SetRotation(const glm::vec3 &rot)
{
	m_Rotate = rot;
}

void Light::SetScale(const glm::vec3 &scale)
{
	m_Scale = scale;
}

void Light::SetLightType(const char* type)
{
	strcpy(m_LightType, type);
}

void Light::SetLightColor(const glm::vec3 color)
{
	m_LightColor = color;
}

void Light::SetLightMoving(const char* moving)
{
	strcpy(m_LightMoving, moving);
}

void Light::SetLightRadius(const float radius)
{
	m_LightRadius = radius;
}

void Light::SetLightSpeed(const float speed)
{
	m_LightSpeed = speed;
}

void Light::Render()
{
	Camera * camera = CameraManager::GetInstance()->GetActiveCamera();//Camera::GetInstance();
	glm::mat4 view = camera->GetWorldToViewMatrix();
	glm::mat4 projector = camera->GetProjector();

	m_Shader->Use();
	m_Shader->EnableState();
	glBindBuffer(GL_ARRAY_BUFFER, m_Model->GetVertexBuffer());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Model->GetIndicesBuffer());

	if (m_Shader->a_posL != -1)
	{
		glEnableVertexAttribArray(m_Shader->a_posL);
		glVertexAttribPointer(m_Shader->a_posL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	//color pos
	if (m_Shader->u_color != -1)
	{
		GLfloat color[] = { m_LightColor.x, m_LightColor.y, m_LightColor.z };

		m_Shader->setUniformMatrix3fv(m_Shader->u_color, color);
	}

	//MVP matrix
	if (m_Shader->u_MVP != -1)
	{
		glm::mat4 MVPMatrix = projector * view * m_WorldMatrix;
		m_Shader->setUniformMatrix4f(m_Shader->u_MVP, MVPMatrix);
	}

	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, m_Model->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_Shader->DisableState();
	m_Shader->UnUse();
}

void Light::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;
	UpdateWorldMatrix();
}

void Light::SetID(const int &id)
{
	m_ObjectID = id;
}

void Light::UpdateWorldMatrix()
{
	/*
	How to orbit object A around object B ?
		-Step 1: Translate A to B.
		-Step 2: Let's A orbit itself
		
	*/
	glm::vec3 center(0.0f);
	if (strcmp(m_LightMoving, "CIRCLE") == 0)
	{		
		if (m_ObjectID == 1)
		{
			center = glm::vec3(-0.2f, 0.0f, 0.01f); 
		}
		else if (m_ObjectID == 0)			 
		{
			center = glm::vec3(0.1f, 0.0f, 0.1f);
		}
		

		m_angle += m_LightSpeed / 100 * m_DeltaTime;
		m_Position.x = m_LightRadius * cos(m_angle) + center.x; // x
		m_Position.z = m_LightRadius * sin(m_angle) + center.z; // z
	
	}
	
	m_WorldMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	m_WorldMatrix = glm::rotate(m_WorldMatrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_WorldMatrix = glm::scale(m_WorldMatrix, m_Scale);
}