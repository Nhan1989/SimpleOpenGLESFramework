#pragma once
#include "../Utilities/utilities.h"
#include "glm/glm.hpp"
#include <vector>
class Shader 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shader();
	Shader();

	void EnableState();
	void DisableState();
	void SetState(const std::string &states);
	void SetState(const char* state);

	void setUniformMatrix4f(GLuint id, glm::mat4 value) const;
	void setUniformMatrix3fv(GLuint id,const GLfloat* pos) const;
	void setUniform1f(GLuint id, const GLfloat value) const;
	GLuint getAttribLocation(const char* name);
	GLuint getUniformLocation(const char* name);
	void Use();
	void UnUse();

	std::vector <std::string> m_State;

	const int MAX_TEXTURE_UNIFORM = 10;

	GLuint a_posL;
	GLuint a_UV;
	GLuint a_normal;
	GLuint a_bitangent;
	GLuint a_tangent;

	GLuint u_modelMatrix;
	GLuint u_MVP;
	GLuint u_camPosition;
	GLuint u_lightPosition;
	GLuint u_time;
	GLuint u_color;
	GLuint u_offset;
	GLuint u_ShadowMap;
	GLuint u_LightSpaceMatrix;
	GLuint u_texLocation[10];

private:
	GLenum GetStateFromChar(const char* state);

};