attribute vec3 a_posL;
attribute vec2 a_UV;



varying vec2 v_UV;




void main()
{
	v_UV = a_UV;
	gl_Position = vec4(a_posL, 1.0);
}
   