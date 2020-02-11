attribute vec3 a_posL;
attribute vec2 a_UV;
attribute vec3 a_normal;

varying vec2 v_UV;

uniform mat4 MVP;

void main()
{
	v_UV = a_UV;
	gl_Position = MVP * vec4(a_posL, 1.0);
}
   