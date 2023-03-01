varying vec3 normal, lightDir, eyeVec;
varying float aoOpacityVarying, aoColorVarying;
uniform sampler3D noiseTexture;
uniform sampler2DRect opacityTexture;
uniform float blendOpacity, 
              aoomPower, aoomThreshold;
uniform bool edgeHighlight, 
			 backfaceOpacity, backfaceOpacityTexture,
		     useTexture, 
		     removeEnclosedAreas,
		     useDepth, useNoise,
             aoColor;

void main() {				
    vec4 color;
	
	// Only ambient occlusion
#ifdef AO_ONLY	
    color.rgb = aoOpacityVarying;
    color.a = 1.0;
#else
	// Per-pixel lighting					
	vec3 N = normalize(normal);		
	vec3 L = normalize(lightDir);
	
	vec3 E = normalize(eyeVec);
	vec3 R = reflect(-L, N);
	
	float lambertTerm = abs(dot(N, L));
	float specularTerm = pow(max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
	
specularTerm = specularTerm * (1.0 - aoOpacityVarying);
	
    // Use per vertex color or not
#ifndef PER_VERTEX_COLOR
//    color = gl_FrontLightModelProduct.sceneColor;    
color = vec4(0.0, 0.0, 0.0, 1.0);
	
#ifdef AOOM_COLOR
    if (aoColor) {
    	color.r = aoColorVarying;
    }
    else {
        color.r = aoOpacityVarying;
    }
#endif // AOOM_COLOR
	
	color += gl_FrontLightProduct[0].ambient;	
	color += gl_FrontLightProduct[0].diffuse * lambertTerm;
	color += gl_FrontLightProduct[0].specular * specularTerm;
	
	
	
	
	color.rgb = mix(vec3(0.0, 0.0, 0.0), vec3(1.0, -0.25, 0.0), aoOpacityVarying);
	color.rgb *= lambertTerm;
	
		color += gl_FrontLightProduct[0].ambient;
		color += gl_FrontLightProduct[0].diffuse * lambertTerm;
		color += gl_FrontLightProduct[0].specular * specularTerm;
	
	
	
#else			
// XXX:HACK FOR NEWEST DATA FROM VYTAS
//gl_Color.b *= 0.75;
    color = gl_Color * gl_LightModel.ambient;
    
    color += gl_FrontLightProduct[0].ambient;	
    color += gl_Color * gl_LightSource[0].diffuse * lambertTerm;
    color += gl_FrontLightProduct[0].specular * specularTerm;
        
#ifdef AOOM_COLOR
    if (aoColor) {
        color.rgb *= 1.0 - aoColorVarying;
    }
    else {
        color.rgb *= 1.0 - aoOpacityVarying;
    }
#endif // AOOM_COLOR
#endif // PER_VERTEX_COLOR

	
	// Texture
    float texColorShift = 0.5;
#ifndef AOOM
	if (useTexture) {
		color.rgb *= texture3D(noiseTexture, gl_TexCoord[0]) * (1.0 - texColorShift) + texColorShift;
		color.rgb *= 1.0 / ((1.0 + texColorShift) * 0.5);
	}
#endif
	
	
	// Render modes
	
#ifdef BLENDED_OPACITY
	color.a = blendOpacity;
#endif // BLENDED_OPACITY

#ifdef AO_LIGHTING
    color.rgb *= aoOpacityVarying;
#endif // AO_LIGHTING

#ifdef AO_BLENDED_OPACITY
	color.rgb *= aoOpacityVarying;
	color.a = blendOpacity;
#endif // AO_BLENDED_OPACITY
	
#ifdef AOOM

	float opacity = 1.0 - aoOpacityVarying;
/*
	if (useDepth) {
		// Use depth for opacity
        useNoise = true;
        if (useNoise ) {
            opacity = 1.0 - length(gl_TexCoord[0] - vec3(0.5, 0.5, 0.5)) / sqrt(3.0) * 2.0;
        }
        else {
            opacity = sqrt(3.0) - length(gl_TexCoord[0] / 100.0) / sqrt(3.0) * 2.0;
        }
    }
	else {
		// Get base opacity from ambient occlusion, passed as gl_Color per vertex
		opacity = 1.0 - aoOpacityVarying;
	}
*/

	// Discard if fully-enclosed
	// XXX:  Currently, this is a hack based on the fact that we are smoothing the 
	// ambient occlusion data, so only fully-enclosed areas will still have an
	// ambient occlusion of 1.0.  Should probably do this geometrically...
	if (removeEnclosedAreas && opacity >= 1.0) {
		discard;
//opacity = 0.5;
	}
	
/*
	// Use power and threshold to set degree of opacity modulation
	//color.a = pow(clamp(opacity / aoomThreshold, 0.0, 1.0), aoomPower);
color.a = clamp(pow(opacity / aoomThreshold, aoomPower), 0.0, 1.0);	
//color.a = smoothstep(midpoint - width * 0.5, midpoint + width * 0.5, opacity);
//color.a = step(power, opacity);	
	
//color.a = abs(color.a - 0.5) * 2.0;
*/
	
	// Other effects
	

	// Texture
/*
    float texScale = 1.0;
    texScale = clamp(texScale, 0.5, 1.0);
	if (useTexture) {
		color.rgb *= texture3D(noiseTexture, gl_TexCoord[0] * texScale) * (1.0 - texColorShift) + texColorShift;
		color.rgb *= 1.0 / ((1.0 + texColorShift) * 0.5);
	}
*/
/*
	// Textured opacity
	float texOpacityScale = texScale;
	if (useTexture) {
        if (useNoise) {
//            color.a = pow(color.a, smoothstep(0.39, 0.41, 1.0 - texture3D(noiseTexture, gl_TexCoord[0] * texOpacityScale)) + 1.0);
        }
        else {
//            color.a = pow(color.a, smoothstep(0.49, 0.51, 1.0 - texture3D(noiseTexture, gl_TexCoord[0] * texOpacityScale)) + 1.0);
        }
	}
*/		


	// Calculate the dot product of the eye and the surface normal
	float eyeDot = dot(N, E);
/*
	// Opacity falloff based on surface normal for edge highlighting
	if (edgeHighlight) {
		color.a = clamp(pow(color.a, abs(eyeDot) + 1.0), 0.0, 1.0);
		color.a = clamp(color.a / abs(eyeDot), 0.0, 1.0);
	}	
*/

	
/*	
	// Further opacity modulation for back-facing surfaces
	float backfaceTexOpacityScale = texOpacityScale;
	if (backfaceOpacity && eyeDot <= 0.0) {
		if (backfaceOpacityTexture) {
            if (useNoise) {
			    color.a *= smoothstep(0.59, 0.61, texture3D(noiseTexture, gl_TexCoord[0] * backfaceTexOpacityScale));
            }
            else {
                color.a *= smoothstep(0.49, 0.51, texture3D(noiseTexture, gl_TexCoord[0] * backfaceTexOpacityScale));
            }
			color.a *= 1.0 - abs(eyeDot) * 0.9;
			color.r += abs(eyeDot); 
		}
		else {
			color.a *= 1.0 - abs(eyeDot) * 0.9;
		}
	}
*/	




	// Opacity projection
/*
	vec4 op = texture2DRect(opacityTexture, gl_FragCoord.st);
	
	float epsilon = 1e-2;	// Why does this need to be a larger value on my desktop machine?
	if (1.0 - aoOpacityVarying >= op.a - epsilon) {
//	if (color.a >= op.b - epsilon) {
		//color.a = 1.0;
		//color.a *= 10.0; 
		//color.a = pow(color.a, 10.0) * 10.0;
	}
	else {
//		color.a *= 0.1;
//		color.a = 0.0;
	}
	
//color = vec4(op.a, op.a, op.a, 1.0);	
*/








vec4 op = texture2DRect(opacityTexture, gl_FragCoord.st);

color.a = pow(opacity / aoomThreshold / op.a, aoomPower);

color.a = clamp(pow(opacity / aoomThreshold, aoomPower), 0.0, 1.0);

color.a = opacity / aoomThreshold;

// Not sure clamping is necessary
color.a = 1.0 - clamp(op.a - color.a, 0.0, 1.0);

// Should be a parameter...
color.a = pow(color.a, aoomPower);



//vec2 co = vec2(-gl_TexCoord[0].s * gl_TexCoord[0].t, gl_TexCoord[0].s * gl_TexCoord[0].t) * 0.0001;
//float n = fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);

//color.a = color.a >= n * 0.5 ? 1.0 : 0.0;

//color.a = max(pow((1.0 - aoOpacityVarying) / aoomThreshold / op.a, 1.0), 0.5);
//color.a = pow(color.a, op.a / color.a);
	
	
//color.a = clamp(pow(color.a, aoomPower), 0.0, 1.0);



//color.a *= clamp(pow(opacity / aoomThreshold, aoomPower), 0.0, 1.0);	



if (useTexture) {
//color.a *= texture3D(noiseTexture, gl_TexCoord[0]) * (1.0 - texColorShift) + texColorShift;
//color.a *= 1.0 / ((1.0 + texColorShift) * 0.5);

//vec3 temp = texture3D(noiseTexture, gl_TexCoord[0]) * (1.0 - texColorShift) + texColorShift;
//		temp.rgb *= 1.0 / ((1.0 + texColorShift) * 0.5);
		
//		color.a *= temp.b;
		
		//color.a = 1.0;
		
//		color.rgba = vec4(1.0, 0.0, 0.0, 1.0);
}



	// Opacity falloff based on surface normal for edge highlighting
	if (edgeHighlight) {
		color.a = clamp(pow(color.a, abs(eyeDot) + 1.0), 0.0, 1.0);
		color.a = clamp(color.a / abs(eyeDot), 0.0, 1.0);

//color.a += (1.0 - abs(eyeDot));		
		
		
//		color.rgb = pow(abs(eyeDot), 2);
//		color.a = 1.0;
	}
	
	
	
//color.rgb = op.a;
//color.a = 1.0;	





	
	
	
	//if (color.a < op.b) color.a = op.b;
	//color.a *= color.a / op.b;
	//if (color.a < op.b) color.a *= 0.1;
	//color.a = pow(color.a, op.b / color.a);
	//if (color.a < op.b - 0.01) discard;
	//color.a = clamp(color.a / op.b + 0.01, 0.0, 1.0);
	//color.a -= (op.b - color.a);
	//color.a = clamp(color.a, 0.0, 1.0);
	//if (color.a < op.b) discard;
	//color.a = max(color.a, 1.0 - op.b);
	
	//color.a = min(color.a, clamp(pow(color.a, pow(op.b, 1.0)), 0.0, 1.0));
	
	//if (color.a < op.b) color.a *= 1.0 - op.b;
	//if (color.a < op.b) color.a = color.a * 0.5;
	//else color.a *= 2.0;
	
//	if (op.b == 1.0) color.a = 0.1;
	
	//color.rgb = op.b;
	//color.a = 1.0;
	
/*	
	if (color.a == op.b) {
		if (op.b < 1.0) {
			color.rgb = 0.5 * abs(eyeDot);
			color.a = 1.0;
		}
	}
	else {
		color.a = 0.0;
	}
*/

/*
	if (abs(eyeDot) < 0.1) {
		color.rgb = 0.5;
		color.a = 1.0;
	}
*/

	
	
	// Border
/*
	int radius = 10;
	int count = 0;	
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			vec4 c = texture2DRect(opacityTexture, vec2(gl_FragCoord.s + i, gl_FragCoord.t + j));
			count += c.r;
		}
	}
	
	int maxCount = (radius * 2 + 1) * (radius * 2 + 1);
	if (count < maxCount) {
//		color.rgb = vec3(0.5, 0.5, 0.5);
		color.a = max(1.0 - (float)count / (float)maxCount, color.a);
		//color.a = 1.0;
//		color.rgb *= count / ((radius + 1) * (radius + 1));
	}
*/


//color.rgb = op.b;
//color.a = 1.0;	


//color.rgb = op.rgb;// * (1.0 - color.a) + color.rgb * color.a;
//color.a = 1.0;

	
// Thresholding
//color.a = gl_Color.a < aoomThreshold;
//color.a += 0.1;


	
	
#endif // AOOM
#endif // AO_ONLY

	gl_FragColor = color;
}