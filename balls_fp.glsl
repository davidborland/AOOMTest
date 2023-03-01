varying vec3 normal, lightDir, eyeVec;

void main() {
	vec4 color;

	// Per-pixel lighting					
	vec3 N = normalize(normal);		
	vec3 L = normalize(lightDir);
	
	vec3 E = normalize(eyeVec);
	vec3 R = reflect(-L, N);
	
	float lambertTerm = abs(dot(N, L));
	float specularTerm = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);

gl_Color.b *= 0.75;		// HACK FOR NEWEST DATA FROM VYTAS
    color = gl_Color * gl_LightModel.ambient;
    
    color += gl_Color * gl_LightSource[0].diffuse * lambertTerm;
    color += gl_FrontLightProduct[0].specular * specularTerm;
    color.a = 1.0;
	
	gl_FragColor = color;
}