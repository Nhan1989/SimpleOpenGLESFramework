precision mediump float;

uniform sampler2D u_texture0;
uniform vec3 u_camPosition; //camera pos
uniform vec3 u_lightPosition;
uniform vec3 u_color;

varying vec2 v_UV;
varying vec3 v_normal;
varying vec3 v_FragPos;

void main()
{
	
	float ambientStrength = 0.2;
	float specularStrength = 1.0;
	
	
	
	vec3 lightColor = vec3(1.0,1.0,1.0);	

	//ambient
	vec3 ambient = ambientStrength * lightColor;
	
	//diffuse
	vec3 norm = v_normal;
	vec3 lightDir = normalize(u_lightPosition - v_FragPos);
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	//specular
	vec3 viewDir = normalize( u_camPosition - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * lightColor;
	
	
	vec3 result = (ambient + diffuse + specular);
	gl_FragColor = vec4(result, 1.0) * texture2D(u_texture0, v_UV) ;

}
