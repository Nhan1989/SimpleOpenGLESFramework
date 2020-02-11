precision mediump float;

uniform sampler2D u_texture0;
varying vec2 v_UV;


void main()
{
	//gl_FragColor = texture2D(u_texture0, v_UV) ;
	 gl_FragColor = vec4(vec3(1.0 - texture2D(u_texture0, v_UV)), 1.0);	
	float average = 0.3 * gl_FragColor.r + 0.59 * gl_FragColor.g + 0.11 * gl_FragColor.b;
	gl_FragColor = vec4(average,average,average,1.0);

}
