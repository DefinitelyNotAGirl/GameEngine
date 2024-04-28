#include <Engine.hxx>
#include <iostream>

using namespace engine;

//,####################################################################################################################
//,####################################################################################################################
//, ███████  ██████   █████  ██    ██ ██████  ███████     ███████  ██████  ██      ██ ██████
//, ██      ██    ██ ██   ██ ██    ██ ██   ██ ██          ██      ██    ██ ██      ██ ██   ██
//, ███████ ██    ██ ███████ ██    ██ ██████  █████       ███████ ██    ██ ██      ██ ██   ██
//,      ██ ██ ▄▄ ██ ██   ██ ██    ██ ██   ██ ██               ██ ██    ██ ██      ██ ██   ██
//, ███████  ██████  ██   ██  ██████  ██   ██ ███████     ███████  ██████  ███████ ██ ██████
//,####################################################################################################################
//,####################################################################################################################
float Phase = 0.5;
float PhaseSpeed = 0.0025;
void __Shader_Generic_Object_Sqaure_Solid__onLoad(Shader* shader, uint64_t zplane)
{
	//std::cout << "support shader load, zplane: " << zplane << std::endl;
	computeLightingForShaderOnZPlane(shader,zplane);
	Phase+=PhaseSpeed;
	if(Phase <= 0.5 || Phase >= 0.6)PhaseSpeed *= -1;
}

Shader __Shader_Generic_Object_Sqaure_Solid(
	"./assets/shaders/Atlas16.vert",
	"./assets/shaders/Atlas16.frag",
	"Generic Atlas 16 Sqaure Shader",
	&__Shader_Generic_Object_Sqaure_Solid__onLoad
);