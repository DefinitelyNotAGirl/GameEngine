#version 460 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D PlayerTexture;

vec4 adjustForLighting(vec4 col);

void main()
{
	vec4 tc = texture(PlayerTexture,texCoord);
	if(tc.a == 0)
	{
		discard;
	}
	FragColor = adjustForLighting(tc);
}