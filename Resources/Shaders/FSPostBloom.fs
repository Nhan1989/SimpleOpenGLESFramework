precision mediump float;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
varying vec2 v_UV;

const float offset = 1.0 / 300.0;  
void main()
{
	
	vec4 color = texture2D(u_texture0, v_UV);
	vec4 blur = texture2D(u_texture1, v_UV);
	gl_FragColor =color+1.2*blur;

}
