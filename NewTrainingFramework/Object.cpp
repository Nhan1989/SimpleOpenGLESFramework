#include "stdafx.h"
#include "Object.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Globals.h"
#include <time.h>
#include "SceneManager.h"
#include "CameraManager.h"
#define SPEED 0.03f

Object::Object()
{
	m_angle = 0.0f;
	
}
Object::~Object()
{

}

void Object::SetObjectID(const int &id)
{
	m_ObjectID = id;
}

int Object::GetObjectID()
{
	return m_ObjectID;
}

void Object::SetModel(const int &id)
{
	m_Model = ResourceManager::GetInstance()->GetModel(id);
}

void Object::SetShader(const int &id)
{
	m_Shader = ResourceManager::GetInstance()->GetShader(id);
}

void Object::SetType(ObjectType type)
{
	m_Type = type;
}

void Object::SetTexture(const int &id)
{
	m_Textures.push_back(ResourceManager::GetInstance()->GetTexture(id));
}

void Object::SetCubeTexture(const int &id)
{
	m_Textures.push_back(ResourceManager::GetInstance()->GetCubeTexture(id));
}

void Object::SetLight(const int &id)
{
	m_LightIDs.push_back(id);
}

void Object::SetPosition(const glm::vec3 &pos)
{
	m_Position = pos;
}

void Object::SetRotation(const glm::vec3 &rot)
{
	m_Rotate = rot;
}

void Object::SetScale(const glm::vec3 &scale)
{
	m_Scale = scale;
}

void Object::UpdateHeightMap(const int &heightMapID)
{
	ReturnUnless(m_Type == ObjectType::TERRAINS);
	Heightmap* hm = ResourceManager::GetInstance()->GetHeightMap(heightMapID);

	std::vector<float> heightMapData = hm->GetHeightMap();
	int row = hm->numRows();
	int colum = hm->numCols();
	m_Model->UpdateHeightMap(heightMapData, row, colum);

}

void Object::Render()
{
	//if (m_Type == ObjectType::TERRAINS) //skip terrians
	//	return;
	Camera * camera = CameraManager::GetInstance()->GetActiveCamera();//Camera::GetInstance();
	glm::mat4 view = camera->GetWorldToViewMatrix();
	glm::mat4 projector = camera->GetProjector();

	m_Shader->Use();
	m_Shader->EnableState();
	glBindBuffer(GL_ARRAY_BUFFER, m_Model->GetVertexBuffer());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Model->GetIndicesBuffer());
	
	//Attribute
	if (m_Shader->a_posL != -1)
	{	
		glEnableVertexAttribArray(m_Shader->a_posL);
		glVertexAttribPointer(m_Shader->a_posL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	
	if (m_Shader->a_UV != -1)
	{
		glEnableVertexAttribArray(m_Shader->a_UV);
		glVertexAttribPointer(m_Shader->a_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
	}	

	if (m_Shader->a_normal != -1)
	{
		glEnableVertexAttribArray(m_Shader->a_normal);
		glVertexAttribPointer(m_Shader->a_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));
	}

	if (m_Shader->a_tangent != -1)
	{
		glEnableVertexAttribArray(m_Shader->a_tangent);
		glVertexAttribPointer(m_Shader->a_tangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));
	}

	if (m_Shader->a_bitangent != -1)
	{
		glEnableVertexAttribArray(m_Shader->a_bitangent);
		glVertexAttribPointer(m_Shader->a_bitangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 11));
	}
	//end attribute

	for (int i = 0; i < m_Textures.size(); i++)
	{
		if (m_Shader->u_texLocation[i] != -1)
		{
			if ((m_Textures[i]->GetTarget() & GL_TEXTURE_2D) == GL_TEXTURE_2D)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_Textures[i]->GetTextureHandle());
				glUniform1i(m_Shader->u_texLocation[i], i);
			}
			/*
			else if ((m_Textures[i]->GetTarget() & GL_TEXTURE_CUBE_MAP) == GL_TEXTURE_CUBE_MAP)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_Textures[i]->GetTextureHandle());
				glUniform1i(m_Shader->u_texLocation[i], i);
			}	
			*/
		}		
	}

	//time
	if (m_Shader->u_time != -1)
	{
		m_Shader->setUniform1f(m_Shader->u_time, SceneManager::GetInstance()->GetGameTime());		
	}

	//MVP matrix
	if (m_Shader->u_MVP != -1)
	{		
		if (m_Type == ObjectType::CUBEMAP)
		{
			view[3][0] = 0; //skip transate for skybox
			view[3][1] = 0;
			view[3][2] = 0;
		}
		glm::mat4 MVPMatrix = projector * view * m_WorldMatrix;
		m_Shader->setUniformMatrix4f(m_Shader->u_MVP, MVPMatrix);
	}

	//model/world matrix
	if (m_Shader->u_modelMatrix != -1)
	{
		m_Shader->setUniformMatrix4f(m_Shader->u_modelMatrix, m_WorldMatrix);
	}

	//camera position
	if (m_Shader->u_camPosition != -1)
	{
		GLfloat cameraPos[] = { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().x };
		m_Shader->setUniformMatrix3fv(m_Shader->u_camPosition, cameraPos);
	}

	//Light position
	if (m_Shader->u_lightPosition != -1)
	{
		Light* light = SceneManager::GetInstance()->GetLight(m_LightIDs[0]);

		GLfloat lightPos[] = { light->GetPosition().x, light->GetPosition().y, light->GetPosition().z};

		m_Shader->setUniformMatrix3fv(m_Shader->u_lightPosition, lightPos);
	}

	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawElements(GL_TRIANGLES, m_Model->GetIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_Shader->DisableState();
	m_Shader->UnUse();
}

void Object::UpdateWorldMatrix()
{
	m_WorldMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	m_WorldMatrix = glm::rotate(m_WorldMatrix, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_WorldMatrix = glm::scale(m_WorldMatrix, m_Scale);	
}

void Object::Update(float deltaTime)
{
	UpdateWorldMatrix();
}