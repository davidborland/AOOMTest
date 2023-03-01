varying vec3 normal, lightDir, eyeVec;
attribute float aoOpacityValue, aoColorValue;
varying float aoOpacityVarying, aoColorVarying;
uniform bool useNoise;

void main() {
	normal = gl_NormalMatrix * gl_Normal;
	
	vec3 position = gl_ModelViewMatrix * gl_Vertex;
	
	lightDir = gl_LightSource[0].position.xyz - position;
	eyeVec = -position;
	
	gl_Position = ftransform();	
	
	gl_FrontColor = gl_Color;
	
	
    aoOpacityVarying = aoOpacityValue;
    aoColorVarying = aoColorValue;

	
	// Compute 3D texture coordinates here.  This is easier than
    // generating correct 2D coordinates for an arbitrary surface,
    // but should look into that at some point.
    useNoise = true;
    if (useNoise) {
    	gl_TexCoord[0] = gl_Vertex + 0.5;
    }
    else {
        gl_TexCoord[0] = gl_Vertex * 100;
    }
}