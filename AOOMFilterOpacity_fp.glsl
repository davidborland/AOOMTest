uniform sampler2DRect texture;

uniform int haloSmoothRadius;

uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main() {
	vec4 color = texture2DRect(texture, gl_FragCoord.st);
/*	
color.a = color.g;
gl_FragColor = color;
return;
*/
	
/*	
	color.a = color.g * weight[0];
	
	vec4 c;
	for (int i = 1; i < 3; i++) {
		c = texture2DRect(texture, vec2(gl_FragCoord) + vec2(0.0, offset[i]));
		color.a += c.g * weight[i];
		
		c = texture2DRect(texture, vec2(gl_FragCoord) - vec2(0.0, offset[i]));
		color.a += c.g * weight[i];
	}
*/

//	int radius = 20;
/*	
	float maxVal = color.g;
	
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			vec4 c = texture2DRect(texture, vec2(gl_FragCoord.s + i, gl_FragCoord.t + j));
			
			if (c.g > maxVal) {
				maxVal = c.g;
			}
		}
	}

	color.a = maxVal;
*/

	float total = 0.0;
	int count = 0;
	for (int i = -haloSmoothRadius; i <= haloSmoothRadius; i++) {
//		for (int j = -radius; j <= radius; j++) {
//int j = 0;
			vec4 c = texture2DRect(texture, vec2(gl_FragCoord.s + i, gl_FragCoord.t));
			
			total += c.b;
			count++;
//		}
	}
	
	color.a = total / count;

	
//color.a = max(color.a, color.g);


//color.a = color.g;

//color.rgb = color.g;
//color.a = 1.0;
	
	gl_FragColor = color;
}