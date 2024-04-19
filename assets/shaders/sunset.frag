#version 460 core
out vec4 FragColor;

vec2 iResolution = vec2(1500.0,900.0);

void main()
{
	vec2 uv;
	uv.x = gl_FragCoord.x / iResolution.x * 2.0 - 1.0;
	uv.y = gl_FragCoord.y / iResolution.y * 2.0 - 0.40;//lower down on screen
	//uv.y = gl_FragCoord.y / iResolution.y * 2.0 - 1.0;//center of screen
	if(uv.y < 0)
	{
		uv.y = uv.y * -1;
	}
	if(uv.x < 0)
	{
		uv.x = uv.x * -1;
	}
	float yo = 1.0-uv.y;
	uv.y = uv.y + 0.35*(length(uv)*2);
	FragColor = vec4(
			((1.0-length(uv))*10)	*(0.1) 	+((yo)*0.50)+ (0.0+length(uv))	*(0.65) /* red 	*/,
			((1.0-length(uv))*10)	*(0.1) 	+((yo)*0.50)+ (0)						/* green*/,
			((1.0-length(uv))*20)	*(0.1) 	-((yo)*1.10)+ (0.0+length(uv))	*(1.15)	/* blue	*/,
		/* inv dist. from 0,0    	| base  | 												*/
		1.0 /* alpha */
	);
}