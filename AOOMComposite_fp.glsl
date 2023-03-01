uniform sampler2DRect texture;

void main() {
	vec4 color = texture2DRect(texture, gl_TexCoord[0].st);

//	color.rgb = color.a;
	
	/*
	int radius = 16;
	float a = 0.0;
	int count = 0;
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			vec4 c = texture2DRect(texture, vec2(gl_TexCoord[0].s + i, gl_TexCoord[0].t + j));
			a += c.a;
			count++;
		}
	}
	
	a /= count;
	
//	color.rgb = a;
color.rgb = abs(color.a - 0.5) * 2;
*/

	
/*
	int radius = 4;

	vec4 color;
				
	color = texture2DRect(texture, gl_TexCoord[0].st);

	int count = 0;
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			vec4 c = texture2DRect(texture, vec2(gl_TexCoord[0].s + i, gl_TexCoord[0].t + j));
			if (c.a <= 0.0) count++;
		}
	}
	
	if (count > 0 && count < radius * radius) {
		color.rgb *= 5;
	}
*/
	gl_FragColor = color;
}