#version 460 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D TextureAtlas;

vec4 adjustForLighting(vec4 col);

void main()
{
	vec4 tc = texture(TextureAtlas,texCoord);
	FragColor = adjustForLighting(tc);
}