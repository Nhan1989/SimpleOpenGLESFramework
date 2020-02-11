attribute vec3 a_posL;
attribute vec2 a_UV;

uniform mat4 MVP;

varying vec2 v_UV;

void main()
{
	v_UV = a_UV;

	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = MVP * posL;
}
   