#include <Engine/Lighting.hxx>
#include <Engine/Object.hxx>
#include <Engine/ObjectClass.hxx>

#include <iostream>

#define ENGINE_BACKEND
#include <Engine.hxx>

namespace engine {
//,####################################################################################################################
//,####################################################################################################################
//, ██      ██  ██████  ██   ██ ████████ ██ ███    ██  ██████  ██████  ██████   ██████  ██████  ███████ ██████  ████████ ██ ███████ ███████
//, ██      ██ ██       ██   ██    ██    ██ ████   ██ ██       ██   ██ ██   ██ ██    ██ ██   ██ ██      ██   ██    ██    ██ ██      ██     
//, ██      ██ ██   ███ ███████    ██    ██ ██ ██  ██ ██   ███ ██████  ██████  ██    ██ ██████  █████   ██████     ██    ██ █████   ███████
//, ██      ██ ██    ██ ██   ██    ██    ██ ██  ██ ██ ██    ██ ██      ██   ██ ██    ██ ██      ██      ██   ██    ██    ██ ██           ██
//, ███████ ██  ██████  ██   ██    ██    ██ ██   ████  ██████  ██      ██   ██  ██████  ██      ███████ ██   ██    ██    ██ ███████ ███████
//,####################################################################################################################
//,####################################################################################################################
	LightingProperties_T::LightingProperties_T(
		rgba Color,
		float Transparency,
		float ReflectionStrength,
		uint64_t zplanes,
		rgba ReflectionMod,
		float LightAngle,
		float Reach
	):
		Color(Color),
		Transparency(Transparency),
		ReflectionStrength(ReflectionStrength),
		zplanes(zplanes),
		ReflectionMod(ReflectionMod),
		LightAngle(LightAngle),
		Reach(Reach)
	{
	}
//,####################################################################################################################
//,####################################################################################################################
//, ██      ██  ██████  ██   ██ ████████ ██ ███    ██  ██████       ██████  ██████  ███    ███ ██████  ██    ██ ████████  █████  ████████ ██  ██████  ███    ██ ███████
//, ██      ██ ██       ██   ██    ██    ██ ████   ██ ██           ██      ██    ██ ████  ████ ██   ██ ██    ██    ██    ██   ██    ██    ██ ██    ██ ████   ██ ██
//, ██      ██ ██   ███ ███████    ██    ██ ██ ██  ██ ██   ███     ██      ██    ██ ██ ████ ██ ██████  ██    ██    ██    ███████    ██    ██ ██    ██ ██ ██  ██ ███████
//, ██      ██ ██    ██ ██   ██    ██    ██ ██  ██ ██ ██    ██     ██      ██    ██ ██  ██  ██ ██      ██    ██    ██    ██   ██    ██    ██ ██    ██ ██  ██ ██      ██
//, ███████ ██  ██████  ██   ██    ██    ██ ██   ████  ██████       ██████  ██████  ██      ██ ██       ██████     ██    ██   ██    ██    ██  ██████  ██   ████ ███████
//,####################################################################################################################
//,####################################################################################################################
	LightingProperties_T* LightingProperties[32];
	RenderPosition2D LightPositions[32];
	void computeLighting(Object* obj)
	{
	}

	void computeLightingForShaderOnZPlane(Shader* shader,uint64_t zplane)
	{
		//std::cout << "computing light for zplane: " << zplane << std::endl;
		uint8_t I = 0;
		for(ObjectClass* c : engine::ObjectClasses)
		{
			if(c->computeLighting == false)continue;//do not scan class for light sources
			for(std::vector<Object*>& __zplane : c->Objects)for(Object* obj : __zplane)
			{
				if(obj->LightingProperties->Color.Alpha == 0)continue;//not a light source
				if((obj->LightingProperties->zplanes & (1<<zplane)) == 0)continue;//light does not act on this z-plane
				//std::cout << "light: 0x" << std::hex << obj->LightingProperties->zplanes << std::dec << " zplane: " << zplane << std::endl;
				LightPositions[I] = RenderPosition2D(obj->Pos);
				RenderSize2D rs(obj->Size);
				LightPositions[I].x += rs.width /2;
				LightPositions[I].y += rs.height/2;
				LightingProperties[I] = obj->LightingProperties;
				I++;
			}
		}
		for(uint64_t i = I;i<32;i++)
		{
			LightingProperties[i] = nullptr;
		}
		GLuint iResolution = glGetUniformLocation(shader->ShaderProgram,"iResolution");
		glUniform2f(iResolution,(float)currentWindow->Width,(float)currentWindow->Height);
		//,
		//, construct arrays to send to GPU
		//,
		float lightPositionArray[32*3];
		float lightColorArray   [32*4];
		{
			for(uint64_t i = 0;i<32;i++)
			{
				float* Position = &lightPositionArray[i*3];
				float* Color = &lightColorArray[i*4];
				if(LightingProperties[i] != nullptr)
				{
					Position[0] = LightPositions[i].x;
					Position[1] = LightPositions[i].y;
					Position[2] = LightingProperties[i]->Reach;
					Color[0] = LightingProperties[i]->Color.Red;
					Color[1] = LightingProperties[i]->Color.Green;
					Color[2] = LightingProperties[i]->Color.Blue;
					Color[3] = LightingProperties[i]->Color.Alpha;
					/*
					std::cout << "light: " << i << std::endl;
					std::cout << "    x: " << Position[0] << std::endl;
					std::cout << "    y: " << Position[1] << std::endl;
					std::cout << "    reach: " << Position[2] << std::endl;
					std::cout << "    r: " << Color[0] << std::endl;
					std::cout << "    g: " << Color[1] << std::endl;
					std::cout << "    b: " << Color[2] << std::endl;
					std::cout << "    a: " << Color[3] << std::endl;
					*/
				}
				else
				{
					Position[0] = 0.0f;
					Position[1] = 0.0f;
					Position[2] = 0.0f;
					Color[0]    = 0.0f;
					Color[1]    = 0.0f;
					Color[2]    = 0.0f;
					Color[3]    = 0.0f;
				}
			}
		}
		//,
		//, send lighting data to GPU
		//,
		{
			GLuint lightPositions = glGetUniformLocation(shader->ShaderProgram,"LightPositions");
			GLuint lightColors = glGetUniformLocation(shader->ShaderProgram,"LightColors");
			glUniform3fv(lightPositions,32,lightPositionArray);
			glUniform4fv(lightColors,32,lightColorArray);
		}
	}
}