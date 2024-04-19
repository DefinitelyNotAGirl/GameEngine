#include <Engine.hxx>
#include <iostream>

using engine::window;
using engine::Position2D;
using engine::Size2D;
using engine::rgba;
using engine::ObjectClass;
using engine::Object;

inline unsigned long long operator""GU(unsigned long long x){return x*  666667ULL;}
inline unsigned long long operator""BM(unsigned long long x){return x*31250000ULL;}

rgba lightColor(0x44,0x33,0x22,0x22);

void game_loop()
{
	return;
}

void game_main()
{
	window* win;
	try {
		 win = new window(Position2D(200,50),Size2D(1500,984),"Game Engine Test");
	} catch (engine::GenericException e) {
		std::cerr << e.what << std::endl;
		std::cerr << "[ERROR] could not create window!" << std::endl;
		exit(-1);
	}
	win->makeCurrent();

	//,
	//, build test level
	//,
	try {
		ObjectClass* Class_Level_Grass 		= engine::getObjectClass("Class.Level.Grass");
		ObjectClass* Class_Level_Rock		= engine::getObjectClass("Class.Level.Rock");
		ObjectClass* Class_Level_Lava		= engine::getObjectClass("Class.Level.Lava");
		ObjectClass* Class_Map_Background 	= engine::getObjectClass("Class.Map.Background");
		//,
		//, background
		//,
		{
			Object* bg = new Object(
				Position2D(0,0),
				Size2D(32BM,32BM),
				Size2D(0,0),
				false,
				false,
				nullptr,
				Class_Map_Background
			);
			engine::PushObject(Class_Map_Background,bg);
		}
		//,
		//, bottom layer
		//,
		for(uint64_t i = 0;i<32;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,0),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Rock
			);engine::PushObject(obj->oclass,obj);
		}
		//,
		//, bottom+1 layer
		//,
		for(uint64_t i = 0;i<32;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,1BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Rock
			);engine::PushObject(obj->oclass,obj);
		}
		//,
		//, bottom+2 layer
		//,
		for(uint64_t i = 0;i<13;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,2BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Grass
			);engine::PushObject(obj->oclass,obj);
			obj = new Object(
				Position2D(i*1BM+19BM,2BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Grass
			);engine::PushObject(obj->oclass,obj);
		}
		Object* obj = new Object(
			Position2D(13BM,2BM),
			Size2D(1BM,1BM),
			Size2D(1BM,1BM),
			false,
			true,
			nullptr,
			Class_Level_Rock
		);engine::PushObject(obj->oclass,obj);
		obj = new Object(
			Position2D(18BM,2BM),
			Size2D(1BM,1BM),
			Size2D(1BM,1BM),
			false,
			true,
			nullptr,
			Class_Level_Rock
		);engine::PushObject(obj->oclass,obj);
		for(uint64_t i = 0;i<4;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM+14BM,2BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Lava
			);engine::PushObject(obj->oclass,obj);
		}
		//,
		//, ceiling
		//,
		for(uint64_t i = 0;i<32;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,20BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Rock
			);engine::PushObject(obj->oclass,obj);
		}
		for(uint64_t i = 0;i<18;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,19BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Rock
			);engine::PushObject(obj->oclass,obj);
		}
		for(uint64_t i = 0;i<15;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,18BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				false,
				true,
				nullptr,
				Class_Level_Rock
			);engine::PushObject(obj->oclass,obj);
		}
	} catch (engine::ClassNotFoundException e) {
		std::cerr << "[ERROR] class not found: " << (char*)e << std::endl;
		exit(-1);
	}
}