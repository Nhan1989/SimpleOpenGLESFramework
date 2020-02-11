#pragma once
//#include "Math.h"
//#include "../Utilities/esMath.h"
#include <glm/glm.hpp>
struct Vertex 
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 norm;
	glm::vec3 tgt;
	glm::vec3 binorm;
	glm::vec3 color;		
};