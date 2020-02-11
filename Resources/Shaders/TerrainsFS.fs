precision mediump float;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_ShadowMap;//shadow map
uniform vec3 u_lightPosition;
uniform vec3 u_camPosition; //camera pos
varying vec4 v_FragPosLightSpace;

uniform vec3 viewPos; //camera pos
varying vec3 v_normal;
//varying vec3 v_FragPos;
varying vec2 v_UV;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords.xyz * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(u_ShadowMap, projCoords.xy).x; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.002;
    float shadow = currentDepth - bias > closestDepth  ? 0.5 : 1.0;

    //return shadow;
	return 1.0;
}


void main()
{
	

	highp vec4 Dirt = texture2D(u_texture0, vec2(v_UV.x, v_UV.y) * 80.0);
	highp vec4 Grass = texture2D(u_texture1, vec2(v_UV.x, v_UV.y) * 80.0);
	highp vec4 Rock = texture2D(u_texture2, vec2(v_UV.x, v_UV.y) * 80.0,-1.0);
	highp vec4 Blend = texture2D(u_texture3, v_UV);
	vec3 color = vec3((Blend.r * Rock  + Blend.g * Grass + Blend.b * Dirt)/ (Blend.r + Blend.g + Blend.b)).rgb;
	
	
	float shadow = ShadowCalculation(v_FragPosLightSpace);
	//gl_FragColor = vec4(0.0,0.0,0.0,0.0);
	gl_FragColor = shadow * ((Blend.r * Rock  + Blend.g * Grass + Blend.b * Dirt)/ (Blend.r + Blend.g + Blend.b)) ;

}
