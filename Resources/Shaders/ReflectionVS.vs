attribute vec3 a_posL;
attribute vec3 a_normal;

varying vec3 v_normW;
varying vec3 v_posW;

uniform mat4 MVP;
uniform mat4 u_modelMatrix;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);

	v_normW = normalize((u_modelMatrix * vec4(a_normal, 0.0)).xyz);
	v_posW = (u_modelMatrix * posL).xyz;

	gl_Position = MVP * posL;	
}
   