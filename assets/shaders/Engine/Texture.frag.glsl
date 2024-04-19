#version 460 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0,texCoord);
	// FragColor = vec4(0.8745, 0.1529, 0.7294, 1.0);
}