#include "stdafx.h"
#include "EffectManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "Globals.h"
#include "SceneManager.h"
#include "FBO.h"
//#include "CameraManager.h"

EffectManager* EffectManager::s_instance = nullptr;



EffectManager::EffectManager():m_CurrentEffect(EEffect::NONE)
{

}

EffectManager::~EffectManager()
{

}

EffectManager* EffectManager::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new EffectManager();
	}
	return s_instance;
}

void EffectManager::CleanUp()
{

}

Effect* EffectManager::GetEffect(const char* name)
{
	auto it = std::find_if(m_Effects.begin(), m_Effects.end(), [name](const Effect* ef)->bool {return ef->effectName == name; });
	if (it != m_Effects.end())
	{
		return (*it);
	}
	return NULL;
}

void EffectManager::SetEffect(EEffect effect)
{
	m_CurrentEffect = effect;
}

EEffect EffectManager::GetEffect()
{
	return m_CurrentEffect;
}

void EffectManager::RenderEffect()
{
	if (m_CurrentEffect != EEffect::NONE)
	{
		switch (m_CurrentEffect)
		{
		case EEffect::BW:
			ActiveEffect("BW");
			break;
		case EEffect::BLUR:
			ActiveEffect("Blur");
			break;
		case EEffect::BLOOM:
			ActiveEffect("Bloom");
			break;
		case EEffect::NONE:
			break;
		default:
			break;
		}
	}
}

/*
	Target 0 - Render directy to screen
	Target 1,2 - Render to screen-off buffer
*/
GLuint EffectManager::ActiveEffect(const char* effectName)
{
	auto ef = std::find_if(m_Effects.begin(), m_Effects.end(), [effectName](const Effect* myEffect)->bool {return myEffect->effectName.compare(effectName) == 0; });
	assert(ef != m_Effects.end()); // not found effect name
	Effect* effect = *ef;
	
	//CameraManager::GetInstance()->SetActiveCamera("main");

	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObjects[0]->GetFBO()); // use first fbo as input
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SceneManager::GetInstance()->Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	for (int i = 0; i < effect->passes.size(); ++i)
	{
		Pass* pass = effect->passes.at(i);		
		Shader* shader = pass->shader;
		shader->Use();
		if (shader->a_posL != -1)
		{
			glEnableVertexAttribArray(shader->a_posL);
			glVertexAttribPointer(shader->a_posL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), quadPositions);
		}

		if (shader->a_UV != -1)
		{
			glEnableVertexAttribArray(shader->a_UV);
			glVertexAttribPointer(shader->a_UV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), quadTexcoords);
		}

		if (shader->u_offset != -1)
		{
			shader->setUniform1f(shader->u_offset, pass->otherData.at(0)); //hard code at index=0
		}

		for (int j = 0; j < pass->textures.size(); ++j)
		{
			glActiveTexture(GL_TEXTURE0 + j);					
			glBindTexture(GL_TEXTURE_2D, m_FrameBufferObjects[pass->textures[j] - 1]->GetColorBuffer()/*m_ColorBuffers[pass->textures[j] - 1]*/);		
		}
		if (pass->target != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObjects[pass->target - 1]->GetFBO()/*m_FrameBufferObjects[pass->target - 1]*/);
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
		}
			
		shader->UnUse();	
	}
	
	return 0;
}

GLuint EffectManager::GetTargetTexture()
{
	return m_CurrentTargetTexture;
}

bool EffectManager::Init(const char* file)
{
	
	std::fstream myFile;
	myFile.open(file);

	if (myFile.is_open())
	{
		std::string line;
		while (std::getline(myFile, line))
		{
			if (line.empty())
				continue;

			if (line.find("#Shaders:") != std::string::npos)
			{
				int numShader;
				sscanf(line.c_str(), "#Shaders: %d", &numShader);
				for (int i = 0; i < numShader; i++)
				{
					int id;
					std::getline(myFile, line); //read id
					sscanf(line.c_str(), "ID %d", &id);
					
					//read filepath
					char vsShader[100];
					char fsShader[100];

					std::getline(myFile, line); //read vs path
					sscanf(line.c_str(), "VSFile: %s", &vsShader);

					std::getline(myFile, line); //read fs path
					sscanf(line.c_str(), "FSFile: %s", &fsShader);
					
					//add shader				
					Shader* shader = new Shader();
					shader->Init(vsShader, fsShader);

					m_Shaders.push_back(shader);
					
				}
			}
			
			if (line.find("#FBO") != std::string::npos)
			{
				int numFBO;
				std::getline(myFile, line);// read number of FBO
				sscanf(line.c_str(), "NoFBO %d", &numFBO);
				//CreateFrameBufferObject(numFBO);

				for (int i = 0; i < numFBO; i++)
				{
					m_FrameBufferObjects[i] = new FBO();
					m_FrameBufferObjects[i]->CreateFBO();
					
				}
				//Special FBO use for depth rendering
				m_FrameBufferObjects[numFBO] = new FBO();
				m_FrameBufferObjects[numFBO]->CreateDepthFBO();
			}
			
			if (line.find("#Effects:") != std::string::npos)
			{
				int numEffect;
				//std::getline(myFile, line);// read number of effect
				sscanf(line.c_str(), "#Effects: %d", &numEffect);
				for (int i = 0; i < numEffect; ++i)
				{
					std::getline(myFile, line);// skip effect id
					Effect* effect = new Effect();
					char effectName[100];
					std::getline(myFile, line);//read name
					sscanf(line.c_str(), "Name %s", &effectName);
					effect->effectName = effectName;

					int numPass;
					std::getline(myFile, line);//read number of pass
					sscanf(line.c_str(), "NoPasses: %d", &numPass);

					for (int j = 0; j < numPass; j++)
					{
						std::getline(myFile, line);//skipp pass id
						Pass* pass = new Pass();

						char passName[100];
						std::getline(myFile, line);//read pass name
						sscanf(line.c_str(), "PassName %s", &passName);
						pass->passName = passName;

						std::getline(myFile, line);//read shader ID
						GLuint shaderID;
						sscanf(line.c_str(), "   ShaderID %d", &shaderID);
						assert(shaderID < m_Shaders.size() && "EffectManger -> Texture ID out of range");
						pass->shader = m_Shaders[shaderID];

						std::getline(myFile, line);//read texture ID (which color buffer is used as input render pass)
						char texture1[100], texture2[100], texture3[100];
						sscanf(line.c_str(), "   Textures: %s\, %s, %s", &texture1, &texture2, &texture3);
						if (strcmp(texture1, "1c,") == 0)
						{
							pass->textures.push_back(1);
						}
						else if (strcmp(texture1, "2c,") == 0)
						{
							pass->textures.push_back(2);
						}
						else if (strcmp(texture1, "3c,") == 0)
						{
							pass->textures.push_back(3);
						}

						if (strcmp(texture2, "1c,") == 0)
						{
							pass->textures.push_back(1);
						}
						else if (strcmp(texture2, "2c,") == 0)
						{
							pass->textures.push_back(2);
						}
						else if (strcmp(texture2, "3c,") == 0)
						{
							pass->textures.push_back(3);
						}

						if (strcmp(texture3, "1c,") == 0)
						{
							pass->textures.push_back(1);
						}
						else if (strcmp(texture3, "2c,") == 0)
						{
							pass->textures.push_back(2);
						}
						else if (strcmp(texture3, "3c,") == 0)
						{
							pass->textures.push_back(3);
						}
						
						std::getline(myFile, line);//read target
						GLuint target;
						sscanf(line.c_str(), "   Target: %d", &target);
						pass->target = target;

						std::getline(myFile, line);//read other data
						float other;
						sscanf(line.c_str(), "   OtherData: %f", &other);
						pass->otherData.push_back(other);

						effect->passes.push_back(pass);
					}
					m_Effects.push_back(effect);
				}
			}

		}
	}
	
	for (auto effect : m_Effects)
	{
		printf("Effect Name %s\n", effect->effectName.c_str());
	}
	
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositions), &quadVertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &quadIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadTexcoords), &quadIndexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return true;
}
