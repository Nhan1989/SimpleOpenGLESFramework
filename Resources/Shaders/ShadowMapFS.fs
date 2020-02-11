precision mediump float;

//uniform vec3 u_color;
uniform sampler2D u_texture0;
varying vec2 v_UV;
void main()
{

	float depth = texture2D(u_texture0, v_UV).r;
	vec3 depthGrayScale = vec3(depth);
	gl_FragColor = vec4(depthGrayScale, 1.0) ;

	//gl_FragColor = vec4(1.0,1.0,1.0, 1.0);
	//float depthValue = texture2D(u_texture0, v_UV).r;
   // gl_FragColor = vec4(vec3(depthValue), 1.0);
}
