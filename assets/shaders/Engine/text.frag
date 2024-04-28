#version 460 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D FontAtlas;
uniform vec4 textColor;

void main()
{
	vec4 tc = texture(FontAtlas,texCoord);
	if(tc.a == 0.0)
	{
		discard;
		//FragColor = vec4(1.0,0.0,0.0,1.0);
		//return;
	}
	FragColor = textColor;
}