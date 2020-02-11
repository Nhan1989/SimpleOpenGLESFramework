#include <stdafx.h>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>



int Shader::Init(char * fileVertexShader,char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);
	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}


	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	a_posL = glGetAttribLocation(program, "a_posL");
	a_UV = glGetAttribLocation(program, "a_UV");
	a_normal = glGetAttribLocation(program, "a_normal");
	a_tangent = glGetAttribLocation(program, "a_tangent");
	a_bitangent = glGetAttribLocation(program, "a_bitangent");

	u_modelMatrix = glGetUniformLocation(program, "u_modelMatrix");
	u_MVP = glGetUniformLocation(program, "MVP");
	u_LightSpaceMatrix = glGetUniformLocation(program, "u_LightSpaceMatrix");
	u_camPosition = glGetUniformLocation(program, "u_camPosition");
	u_lightPosition = glGetUniformLocation(program, "u_lightPosition");
	u_time = glGetUniformLocation(program, "u_time");
	u_color = glGetUniformLocation(program, "u_color");
	u_offset = glGetUniformLocation(program, "u_offset");
	u_ShadowMap = glGetUniformLocation(program, "u_ShadowMap");

	for (int i = 0; i < MAX_TEXTURE_UNIFORM; i++)
	{
		char uniformTexName[20];
		sprintf(uniformTexName, "u_texture%d", i);
		u_texLocation[i] = glGetUniformLocation(program, uniformTexName);
	}
	return 0;
}
Shader::Shader()
{

}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(program);
}

void Shader::UnUse()
{
	glUseProgram(0);
} 
void Shader::SetState(const std::string &states)
{
	m_State.push_back(states);
}
void Shader::EnableState()
{
	for (auto state : m_State)
	{
		glEnable(GetStateFromChar(state.c_str()));
	}
}

void Shader::DisableState()
{
	for (auto state : m_State)
	{
		glDisable(GetStateFromChar(state.c_str()));
	}
}

void Shader::SetState(const char* state)
{
	m_State.push_back(state);
}

GLenum Shader::GetStateFromChar(const char* state)
{
	GLenum bitmask = 0;
	std::string myState(state);

	if (myState.compare("CULL_FACE") == 0)
	{
		bitmask |= GL_CULL_FACE;
	}
	else if (myState.compare("DEPTH_TEST") == 0)
	{
		bitmask |= GL_DEPTH_TEST;
	}
	else if (myState.compare("BLEND") == 0)
	{
		bitmask |= GL_BLEND;
	}
	return bitmask;
}

GLuint Shader::getAttribLocation(const char* name)
{
	return glGetAttribLocation(program, name);
}
GLuint Shader::getUniformLocation(const char* name)
{
	return glGetUniformLocation(program, name);
}
void Shader::setUniformMatrix4f(GLuint id, glm::mat4 value) const
{
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform1f(GLuint id, GLfloat value) const
{
	glUniform1f(id, value);
}
void Shader::setUniformMatrix3fv(GLuint id, const GLfloat* pos) const
{
	glUniform3fv(id, 1, pos);
}