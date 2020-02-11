attribute vec3 a_posL;
attribute vec2 a_UV;
attribute vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 MVP;

uniform mat4 u_LightSpaceMatrix; //LightProjector * LightView

varying vec2 v_UV;
varying vec3 v_normal;
//varying vec3 v_FragPos;
varying vec4 v_FragPosLightSpace;




void main()
{
	v_UV = a_UV;
	v_normal = a_normal;
	//v_FragPos = vec3(u_modelMatrix * vec4(a_posL,1.0));
	v_FragPosLightSpace = u_LightSpaceMatrix * u_modelMatrix  * vec4(a_posL, 1.0);

	gl_Position = MVP * vec4(a_posL, 1.0);
}
   