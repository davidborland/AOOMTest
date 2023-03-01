varying vec3 normal, lightDir, eyeVec;

void main() {
	// Per-pixel lighting
	vec3 N = normalize(normal);		
	vec3 L = normalize(lightDir);
	
	vec3 E = normalize(eyeVec);
	vec3 R = reflect(-L, N);
	
	float lambertTerm = abs(dot(N, L));
	float specularTerm = pow(max(abs(dot(R, E)), 0.0), gl_FrontMaterial.shininess);
	
	vec4 color = gl_FrontLightModelProduct.sceneColor;
	color += gl_FrontLightProduct[0].ambient;
	color += gl_FrontLightProduct[0].diffuse * lambertTerm;
	color += gl_FrontLightProduct[0].specular * specularTerm;
	
	gl_FragColor = color;
}