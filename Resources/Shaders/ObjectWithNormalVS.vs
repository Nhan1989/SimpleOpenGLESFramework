attribute vec3 a_posL;
attribute vec2 a_UV;
attribute vec3 a_normal;
attribute vec3 a_tangent;
attribute vec3 a_bitangent;


varying vec2 v_UV;
varying vec3 v_normalW;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying vec3 v_FragPos;

uniform mat4 MVP;
uniform mat4 u_modelMatrix;

void main()
{
	gl_Position = MVP * vec4(a_posL, 1.0);
	v_UV = a_UV;
	v_FragPos = vec3(u_modelMatrix * vec4(a_posL,1.0));
	v_normalW = normalize(u_modelMatrix * vec4(a_normal, 0.0)).xyz;
    v_tangentW = normalize(u_modelMatrix * vec4(a_tangent, 0.0)).xyz;
    v_bitangentW = normalize(u_modelMatrix * vec4(a_bitangent, 0.0)).xyz;

}
   