precision mediump float;

uniform sampler2D u_texture0; //object texture
uniform sampler2D u_texture1; //normalmap
uniform vec3 u_lightPosition;

varying vec2 v_UV;
varying vec3 v_normalW;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying vec3 v_FragPos;

void main()
{
	vec3 normal = texture2D(u_texture1,v_UV).xyz;
    mat3 TBN = mat3(normalize(v_tangentW),normalize(v_bitangentW),normalize(v_normalW));
    vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
	vec3 lightDir = normalize(v_FragPos - u_lightPosition);
    float diffuse = max(0.0, dot(-lightDir,normalW));

    vec4 color = texture2D(u_texture0,v_UV);
    gl_FragColor = vec4(diffuse * color.xyz,color.a) ;
}
