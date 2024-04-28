uniform vec3 LightPositions[32];
uniform vec4 LightColors[32];
uniform vec2 iResolution;


vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy) / iResolution.y;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43.5453);
}

vec3 adjustForLight(vec3 colIN, int n)
{
	vec2 lp = (LightPositions[n].xy) * (iResolution.xy/iResolution.y);
	vec4 lightColor = LightColors[n];
	if(lightColor.a == 0.0)return colIN;
	float LD = 1.0-distance(uv,lp)*(LightPositions[n].z);
	if(LD <= 0.0)return colIN;
	vec3 colOUT = colIN*(1.0+lightColor.rgb*LD*lightColor.a);
	return colOUT;
}

vec4 adjustForLighting(vec4 colIN)
{
	//return vec4(uv.xy,1.0,1.0);
	vec3 colOUT = colIN.rgb;
	for(int i = 0;i<32;i++)
	{
		colOUT = adjustForLight(colOUT,i);
	}
	return vec4(colOUT-colIN.rgb,colIN.a);
}
