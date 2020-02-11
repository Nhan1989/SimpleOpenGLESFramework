precision mediump float;
varying vec3 texCoord;
uniform samplerCube u_texture0;
void main()
{
	

	mediump vec3 cube = vec3(textureCube(u_texture0, texCoord));
    gl_FragColor = vec4(cube, 1.0);
   

}
