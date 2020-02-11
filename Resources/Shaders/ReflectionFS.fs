precision mediump float;

uniform samplerCube u_texture0;
uniform vec3 u_camPosition;

varying vec3 v_normW;
varying vec3 v_posW;

void main()
{
	vec3 toEye = u_camPosition - v_posW;
	vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normW));
	gl_FragColor = textureCube(u_texture0,reflectDir);
}
