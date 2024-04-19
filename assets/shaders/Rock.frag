#version 460 core
out vec4 FragColor;

float random (vec2 st) {
    return fract(sin(dot(st.xy,vec2(0.345456645,543.456546564)))*59900000.0);
}

void main()
{
	vec2 st = gl_FragCoord.xy/(1.0,1.0);
    float rnd = random(st);
    FragColor = vec4(vec3(rnd)-0.5,1.0);
}