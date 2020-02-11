precision mediump float;

uniform sampler2D u_texture0;
varying vec2 v_UV;

uniform float u_offset;
void main()
{
	
	//float limit = 0.5;
	vec3 color = texture2D(u_texture0, v_UV).rgb;
	float brightness = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	float val = step(u_offset, brightness);
	gl_FragColor = vec4(color * val, 1.0);

}
