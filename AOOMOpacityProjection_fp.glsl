varying vec3 normal, lightDir, eyeVec;
varying float aoOpacityVarying;
uniform float aoomPower, aoomThreshold;
uniform bool edgeHighlight,
		     removeEnclosedAreas;

void main() {

/*
	// Get base opacity from ambient occlusion, passed as gl_Color per vertex
	float opacity = 1.0 - aoOpacityVarying;
	

	// Use power and threshold to set degree of opacity modulation
	opacity = clamp(pow(opacity / aoomThreshold, aoomPower), 0.0, 1.0);		
	
	
	// Other effects
	
	
	vec3 N = normalize(normal);	
	vec3 E = normalize(eyeVec);
	
		
	// Calculate the dot product of the eye and the surface normal
	float eyeDot = dot(N, E);

	// Opacity falloff based on surface normal for edge highlighting
	if (edgeHighlight) {
		opacity = clamp(pow(opacity, abs(eyeDot) + 1.0), 0.0, 1.0);
	}		
	

	// Per-pixel lighting					
//	vec3 N = normalize(normal);		
	vec3 L = normalize(lightDir);
	
//	vec3 E = normalize(eyeVec);
	vec3 R = reflect(-L, N);
	
	float lambertTerm = abs(dot(N, L));
	float specularTerm = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
	
	vec4 color = gl_FrontLightModelProduct.sceneColor;
	
//	color = vec4(0.9, 0.9, 0.9, 1.0);
	color.r = 0.6;
	
	color += gl_FrontLightProduct[0].ambient;	
	color += gl_FrontLightProduct[0].diffuse * lambertTerm;
	color += gl_FrontLightProduct[0].specular * specularTerm;	
	
	color.a = 1.0;
	
	gl_FragColor = color;
*/	
	


	// Get base opacity from ambient occlusion, passed as gl_Color per vertex
	float opacity = 1.0 - aoOpacityVarying;
	
	
	// Discard if fully-enclosed
	// XXX:  Currently, this is a hack based on the fact that we are smoothing the 
	// ambient occlusion data, so only fully-enclosed areas will still have an
	// ambient occlusion of 1.0.  Should probably do this geometrically...
	if (removeEnclosedAreas && opacity >= 1.0) {
		discard;
//opacity = 0.5;
	}
	
aoomPower = 1.0;

	// Use power and threshold to set degree of opacity modulation
	opacity = clamp(pow(opacity / aoomThreshold, aoomPower), 0.0, 1.0);		
	
	
	// Other effects
	
	
	vec3 N = normalize(normal);	
	vec3 E = normalize(eyeVec);
	
		
	// Calculate the dot product of the eye and the surface normal
	float eyeDot = dot(N, E);

	// Opacity falloff based on surface normal for edge highlighting
	if (edgeHighlight) {
		opacity = clamp(pow(opacity, abs(eyeDot) + 1.0), 0.0, 1.0);
	}		
	

	gl_FragColor.rgb = vec3(1.0, 1.0 - aoOpacityVarying, opacity);	
}