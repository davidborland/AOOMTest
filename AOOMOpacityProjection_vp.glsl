varying vec3 normal, lightDir, eyeVec;
attribute float aoOpacityValue, aoColorValue;
varying float aoOpacityVarying;

void main() {
	normal = gl_NormalMatrix * gl_Normal;
	
	vec3 position = gl_ModelViewMatrix * gl_Vertex;
		
	lightDir = gl_LightSource[0].position.xyz - position;
	eyeVec = -position;
	
	gl_Position = ftransform();	
	
	
    aoOpacityVarying = aoOpacityValue;
}