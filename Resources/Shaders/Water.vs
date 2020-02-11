attribute vec3 a_posL;
attribute vec2 a_UV;
attribute vec3 a_normal;
attribute vec3 a_tangent;
attribute vec3 a_bitangent;

varying vec2 vUV;
varying vec3 vPosW;
varying vec3 vNormalW;
varying vec3 vTangentW;
varying vec3 vBinormalW;

uniform mat4 MVP;
uniform mat4 u_modelMatrix;
	

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	
	gl_Position = MVP * posL;
	
	vUV = a_UV;
	vPosW = (u_modelMatrix * posL).xyz;
	vNormalW = (u_modelMatrix * vec4(a_normal, 0.0)).xyz;
	vTangentW = (u_modelMatrix * vec4(a_tangent, 0.0)).xyz;
	vBinormalW = (u_modelMatrix * vec4(a_bitangent, 0.0)).xyz;
}
   