#version 460 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D TextureAtlas;

void main()
{
	vec4 tc = texture(TextureAtlas,texCoord);
	FragColor = vec4(
		tc.r,
		tc.g,
		tc.b,
		tc.a
	);
}