#pragma once

#include <vector>
#include "../Utilities/utilities.h"
#include "Shader.h"
#include <map>
#include "FBO.h"
struct Pass 
{
	std::string passName;
	Shader* shader;
	std::vector<GLuint> textures;//list texture
	int target; // frame buffer target
	std::vector<float> otherData;
};

struct Effect
{
	std::string effectName;
	std::vector<Pass*> passes;
};

enum class EEffect
{
	BW,
	BLUR,
	BLOOM,
	NONE
};

typedef std::vector<Effect*> Effects;
typedef std::vector<Shader*> Shaders;
typedef std::vector<GLuint> FrameBufferObjects;

class EffectManager
{
public:
	static EffectManager* GetInstance();
	bool Init(const char* file);

	Effect* GetEffect(const char* name);
	GLuint ActiveEffect(const char* effectName);
	GLuint GetTargetTexture();
	void SetEffect(EEffect effect);
	EEffect GetEffect();
	void RenderEffect();
	void CleanUp();

	
private:
	EffectManager();
	~EffectManager();

	//Shaders m_Shaders;
	FBO* m_FrameBufferObjects[4];
	Effects m_Effects;
	Shaders m_Shaders;
	GLuint m_CurrentTargetTexture;
	GLuint quadVertexBuffer, quadIndexBuffer;
	bool m_IsActiveEffect;
	EEffect m_CurrentEffect;

	static EffectManager* s_instance;
};