#include <Engine.hxx>
#include <iostream>

using engine::window;
using engine::Position2D;
using engine::Size2D;
using engine::rgba;
using engine::ObjectClass;
using engine::Object;

#include <GLFW/glfw3.h>

inline unsigned long long operator""GU(unsigned long long x){return x*  666667ULL;}
inline unsigned long long operator""BM(unsigned long long x){return x*31250000ULL;}

rgba lightColor(0x44,0x33,0x22,0x22);

ObjectClass* Class_Level_Portal;
ObjectClass* Class_Level_Lava;

float AnimationPhase = 0.5;
float AnimationPhaseSpeed = 0.01;

Object* PlayerObject;

uint8_t playerAnimY = 10;// 10 = idle | 9 = run | 7 = slide | 8 = jump
uint8_t playerAnimX =  0;//  0 = idle | 1 = run | 3 = slide | 3 = jump

void killPlayer()
{
	PlayerObject->Pos = Position2D(18BM,3BM);
}

bool movingRight = false;
bool movingLeft = false;
int64_t velocityY = 0;
int64_t PlayerSpeed = 2GU;
bool crouching = false;
bool sprinting = false;
bool flipPlayer = false;
void setMoveAnimation()
{
	if(velocityY != 0)
	{
		playerAnimY = 8;
		playerAnimX = 3;
		return;
	}
	if(movingLeft || movingRight)
	{
		if(crouching)
		{
			PlayerSpeed = 1GU;
			playerAnimY = 1;
			playerAnimX = 2;
		}
		else if(sprinting)
		{
			PlayerSpeed = 3GU;
			playerAnimY = 9;
			playerAnimX = 1;
		}
		else
		{
			PlayerSpeed = 2GU;
			playerAnimY = 2;
			playerAnimX = 3;
		}
	}
	else
	{
		if(crouching)
		{
			playerAnimY = 5;
			playerAnimX = 3;
		}
		else
		{
			playerAnimY = 5;
			playerAnimX = 3;
		}
	}
}
uint64_t doubleJumpCount = 0;
void resetPlayerJumpAnimation()
{
	doubleJumpCount = 0;
	setMoveAnimation();
}
void keyDownShift()
{
	sprinting = true;
	setMoveAnimation();
}
void keyUpShift()
{
	sprinting = false;
	setMoveAnimation();
}
void keyDownCtrl()
{
	crouching = true;
	setMoveAnimation();
}
void keyUpCtrl()
{
	crouching = false;
	setMoveAnimation();
}
void keyDownD()
{
	flipPlayer = false;
	movingRight=true;
	setMoveAnimation();
}
void keyUpD()
{
	movingRight=false;
	setMoveAnimation();
}
void keyDownA()
{
	flipPlayer = true;
	movingLeft=true;
	setMoveAnimation();
}
void keyUpA()
{
	movingLeft=false;
	setMoveAnimation();
}

void keyDownSpace()
{
	if(velocityY != 0)
	{
		if(doubleJumpCount >= 1)
			return;
		doubleJumpCount++;
	}
	velocityY = 4GU;
	setMoveAnimation();
}

bool playerDoGravity = false;
void game_loop()
{
	//,
	//, check if player has fallen out of the world
	//,
	{
		if(PlayerObject->Pos.y >= 18000000000000000000ULL)
		{
			PlayerObject->Pos = Position2D(18BM,3BM);
		}
	}
	//,
	//, player gravity
	//,
	{
		if(playerDoGravity)
		{
			velocityY -= 50000;
		}
	}
	playerDoGravity = true;
	if(AnimationPhase <= 0 || AnimationPhase >= 1)
		AnimationPhaseSpeed*=-1;
	AnimationPhase+=AnimationPhaseSpeed;
	Class_Level_Lava->LightingProperties->Reach   = 1.125f+(AnimationPhase/16);
	Class_Level_Lava->LightingProperties->Color.Green += (AnimationPhaseSpeed/8);
	Class_Level_Portal->LightingProperties->Reach = 4.000f+AnimationPhase;
	//,####################################################################################################################
	//,####################################################################################################################
	//, ███    ███  ██████  ██    ██ ███████ ███    ███ ███████ ███    ██ ████████
	//, ████  ████ ██    ██ ██    ██ ██      ████  ████ ██      ████   ██    ██
	//, ██ ████ ██ ██    ██ ██    ██ █████   ██ ████ ██ █████   ██ ██  ██    ██
	//, ██  ██  ██ ██    ██  ██  ██  ██      ██  ██  ██ ██      ██  ██ ██    ██
	//, ██      ██  ██████    ████   ███████ ██      ██ ███████ ██   ████    ██
	//,####################################################################################################################
	//,####################################################################################################################
	if(movingRight == true)
	{
		PlayerObject->Pos.x += PlayerSpeed;
	}
	if(movingLeft == true)
	{
		PlayerObject->Pos.x -= PlayerSpeed;
	}
	PlayerObject->Pos.y += velocityY;
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

	engine::initKeyCallbacks(win);
	engine::setKeyUpCallback(GLFW_KEY_LEFT_CONTROL,&keyUpCtrl);
	engine::setKeyDownCallback(GLFW_KEY_LEFT_CONTROL,&keyDownCtrl);
	engine::setKeyUpCallback(GLFW_KEY_LEFT_SHIFT,&keyUpShift);
	engine::setKeyDownCallback(GLFW_KEY_LEFT_SHIFT,&keyDownShift);
	engine::setKeyUpCallback(GLFW_KEY_D,&keyUpD);
	engine::setKeyDownCallback(GLFW_KEY_D,&keyDownD);
	engine::setKeyUpCallback(GLFW_KEY_A,&keyUpA);
	engine::setKeyDownCallback(GLFW_KEY_A,&keyDownA);
	engine::setKeyDownCallback(GLFW_KEY_SPACE,&keyDownSpace);

	//,
	//, build test level
	//,
	try {
		engine::setZPlaneCount(4);
		ObjectClass* Class_Player 				= engine::getObjectClass("Class.Player");
		ObjectClass* Class_Level_Rock			= engine::getObjectClass("Class.Level.Rock");
		ObjectClass* Class_Level_DeepRock 		= engine::getObjectClass("Class.Level.DeepRock");
		ObjectClass* Class_Level_AmbientLight	= engine::getObjectClass("Class.Level.AmbientLight");
		Class_Level_Portal 	= engine::getObjectClass("Class.Level.Portal");
		Class_Level_Lava	= engine::getObjectClass("Class.Level.Lava");
		ObjectClass* Class_Map_Background = engine::getObjectClass("Class.Map.Background");
		//,
		//,	load textures
		//,
		{
			engine::simpleLoadTexture("./assets/Textures/TestAtlas.png",GL_TEXTURE0);
			engine::simpleLoadTexture("./assets/Textures/adventurer-Sheet.png",GL_TEXTURE1);
		}
		//,
		//, spawn player
		//,
		{
			Object* obj = new Object(
				Position2D(8BM,3BM),
				Size2D(100GU,74GU),
				Size2D(100GU,74GU),
				1,
				nullptr,
				Class_Player
			);engine::PushObjectGlobal(obj);
			PlayerObject = obj;
		}
		//,
		//, ambient light
		//,
		{
			Object* obj = new Object(
				Position2D(0,0),
				Size2D(1,1),
				Size2D(0,0),
				3,
				nullptr,
				Class_Level_AmbientLight
			);engine::PushObjectGlobal(obj);
		}
		//,
		//, background
		//,
		{
			for(uint64_t y = 0;y<20;y++)
			{
				for(uint64_t x = 0;x<32;x++)
				{
					ObjectClass* oc = ((y == 3 || y == 4) && (x == 31 || x == 30)) ? Class_Level_Portal : Class_Level_DeepRock;
					Object* obj = new Object(
						Position2D(x*1BM,y*1BM),
						Size2D(1BM,1BM),
						Size2D(1BM,1BM),
						0,
						nullptr,
						oc
					);engine::PushObjectGlobal(obj);
				}
			}
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
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
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
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
		}
		//,
		//, platforms
		//,
		{
			Object* obj = new Object(
				Position2D(29BM,5BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
			obj = new Object(
				Position2D(30BM,5BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
			obj = new Object(
				Position2D(31BM,5BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
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
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
			obj = new Object(
				Position2D(i*1BM+19BM,2BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
		}
		Object* obj = new Object(
			Position2D(13BM,2BM),
			Size2D(1BM,1BM),
			Size2D(1BM,1BM),
			1,
			nullptr,
			Class_Level_Rock
		);engine::PushObjectGlobal(obj);
		obj = new Object(
			Position2D(18BM,2BM),
			Size2D(1BM,1BM),
			Size2D(1BM,1BM),
			1,
			nullptr,
			Class_Level_Rock
		);engine::PushObjectGlobal(obj);
		for(uint64_t i = 0;i<4;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM+14BM,2BM-2GU),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Lava
			);engine::PushObjectGlobal(obj);
		}
		//,
		//, pillars
		//,
		{
			for(uint64_t x : std::vector<uint64_t>({2,5,10,15}))
			for(uint64_t y = 0;y<20;y++)
			{
				Object* obj = new Object(
					Position2D(x*1BM,y*1BM),
					Size2D(1BM,1BM),
					Size2D(1BM,1BM),
					3,
					nullptr,
					Class_Level_Rock
				);engine::PushObjectGlobal(obj);
			}
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
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
		}
		for(uint64_t i = 0;i<18;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,19BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
		}
		for(uint64_t i = 0;i<15;i++)
		{
			Object* obj = new Object(
				Position2D(i*1BM,18BM),
				Size2D(1BM,1BM),
				Size2D(1BM,1BM),
				1,
				nullptr,
				Class_Level_Rock
			);engine::PushObjectGlobal(obj);
		}
	} catch (engine::ClassNotFoundException e) {
		std::cerr << "[ERROR] class not found: " << (char*)e << std::endl;
		exit(-1);
	}
}