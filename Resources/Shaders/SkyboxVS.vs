attribute vec3 a_posL;
attribute vec2 a_UV;
varying vec3 texCoord;

uniform mat4 MVP;
void main()
{

texCoord = a_posL.xyz;
vec4 posL = vec4(a_posL, 1.0);
gl_Position = MVP * posL;
gl_Position=gl_Position.xyww;
}
   