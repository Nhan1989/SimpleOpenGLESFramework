precision mediump float;

uniform sampler2D u_texture0; //displacement
uniform sampler2D u_texture1; //fire
uniform sampler2D u_texture2; //fire mask
uniform float u_time;

varying vec2 v_UV;

void main()
{
	vec2 displacement = texture2D(u_texture0, vec2(v_UV.x, v_UV.y + u_time)).rg * 0.4 - 0.2;
	vec4 fireColor = texture2D(u_texture1, v_UV + displacement);
	vec4 mask = texture2D(u_texture2, v_UV);
	gl_FragColor = fireColor * vec4(1.0, 1.0, 1.0, mask.r);
/*
	float dMax = 0.5;
	vec2 disp = texture2D(u_texture0, vec2(v_UV.x, v_UV.y + u_time)).xy;

	vec2 offset = (2.0 * disp - 1.0) * dMax;

	vec2 uv_displaced = v_UV + offset;
	vec4 fire_color = texture2D(u_texture1, uv_displaced);

	vec4 AlphaValue = texture2D(u_texture2, v_UV);
	gl_FragColor = fire_color * (1.0, 1.0, 1.0, AlphaValue.r);

*/	
	
	if(gl_FragColor.a < 0.5)
	{
		discard;
	}
}
