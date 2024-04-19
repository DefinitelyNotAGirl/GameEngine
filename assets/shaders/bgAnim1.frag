#version 460 core
out vec4 FragColor;

vec2 iResolution = vec2(1500.0,900.0);
uniform float frame;
uniform float iTime;

vec3 palette(float t)
{
	vec3 a = vec3(0.5,0.5,0.5);
	vec3 b = vec3(0.5,0.5,0.5);
	vec3 c = vec3(1.0,1.0,1.0);
	vec3 d = vec3(0.263,0.416,0.557);
	return a + b*cos(6.28318*(c*t+d));
}

void main()
{
	vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy) / iResolution.y;
	vec2 uv0 = uv;
	vec3 finalColor = vec3(0.0);

	for(float i = 0.0; i < 3.0; i++)
	{
		uv = fract(uv * (2.0)) - 0.5;
		float d = length(uv) * exp(-length(uv0));
		vec3 col = palette(length(uv0));
		float rs = 8.0;
		d = sin(d*rs+(iTime/8))/rs;
		d = abs(d);
		d = pow(0.01 / d, 2.0);
		finalColor += col * d;
	}

	FragColor = vec4(finalColor, 1.0);
}