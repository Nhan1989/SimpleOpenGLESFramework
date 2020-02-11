precision mediump float;

varying vec2 vUV;
varying vec3 vPosW;

uniform sampler2D u_texture0; // the rock
uniform sampler2D u_texture1; // displacement
uniform sampler2D u_texture2; // normal map
uniform sampler2D u_texture3; // depth map
uniform sampler2D u_texture4; // depth map
uniform samplerCube uTextureCube0; // the sky box

uniform float u_time;//Game time
uniform vec3 u_camPosition;

varying vec3 vNormalW;
varying vec3 vTangentW;
varying vec3 vBinormalW;

void main()
{		

	vec4 waterColor = vec4(0.1, 0.4, 0.65, 0.0);
    
    
	vec2 displacement = texture2D(u_texture1, vec2(vUV.x, vUV.y + u_time)).rg * 0.2 - 0.1;
	float depth = texture2D(u_texture3, vec2(vUV.x / 5.0, vUV.y / 3.0)).r;
	
	vec3 toEye = normalize(vPosW - u_camPosition);
	
	mat3 TBN = mat3(normalize(vTangentW), normalize(vBinormalW), normalize(vNormalW));
	vec3 normal = texture2D(u_texture2, vUV + displacement).xyz;
	vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
	
    
	float f = pow((1.0 - abs(dot(normalW, toEye))), 4.0);
	
    
	vec3 reflectVector = reflect(toEye, normalW);
	vec4 reflectColor = textureCube(uTextureCube0, reflectVector);
	
    
	vec4 rockColor = texture2D(u_texture4, vUV + depth * displacement);
    
	gl_FragColor = mix(mix(rockColor, waterColor, depth), reflectColor, f);
    
    
}
