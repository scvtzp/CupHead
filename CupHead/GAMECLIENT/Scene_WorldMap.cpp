#include "Scene_WorldMap.h"
#include <Logic_Enum.h>
#include <Player_Map.h>
#include <LOGIC_Value.h>
#include <Map_Entrance.h>
#include <Inventory.h>
#include <Game_3D_Debug.h>

#define Shift_Icon_Size {60.f, 60.f, 1.f}
#define Shift_Icon_LPos_Y -150.f

void Scene_WorldMap::Init()
{
	SCENE()->ColLink((int)COLORDER::COLORDER_PLAYER_ForUnknown
		, (int)COLORDER::Map_Entrance);
}

void Scene_WorldMap::Update()
{
	if(Game_Input::Down(L"JUMP"))
		Noise->TRANS()->WPOS({ 0.f, 0.f, Noise->TRANS()->WPOS().Z - 10.f });

	if (Game_Input::Down(L"Dash")) //쉬프트
	{
		{
			if (ShiftCheck)
			{
				ShiftCheck = false;
				Player_Map::MoveCheck = true;
				Shift_Renderer->Off();
				Shift_Renderer_1->Off();
				Shift_Renderer_2->Off();
				Shift_Renderer_3->Off();
			}
			else
			{
				Shift_Renderer->On();
				Shift_Renderer_1->On();
				Shift_Renderer_2->On();
				Shift_Renderer_3->Off();
				ShiftCheck = true;
				Player_Map::MoveCheck = false;
			}
		}
	}
	if (ShiftCheck)
	{
		Inventory::Change_Inventory_Type();

		switch (Inventory::Inven_1)
		{
		case Inventory_Bullet_Type::Peashot:
			Shift_Renderer_1->SPRITE(L"equip_icon_weapon_peashot.png");
			break;
		case Inventory_Bullet_Type::Spread:
			Shift_Renderer_1->SPRITE(L"equip_icon_weapon_spread.png");
			break;
		case Inventory_Bullet_Type::Bouncer:
			Shift_Renderer_1->SPRITE(L"equip_icon_weapon_bouncer.png");
			break;
		case Inventory_Bullet_Type::Charge:
			Shift_Renderer_1->SPRITE(L"equip_icon_weapon_charge.png");
			break;
		default:
			Shift_Renderer_1->SPRITE(L"equip_icon_empty.png");
			break;
		}

		switch (Inventory::Inven_2)
		{
		case Inventory_Bullet_Type::Peashot:
			Shift_Renderer_2->SPRITE(L"equip_icon_weapon_peashot.png");
			break;
		case Inventory_Bullet_Type::Spread:
			Shift_Renderer_2->SPRITE(L"equip_icon_weapon_spread.png");
			break;
		case Inventory_Bullet_Type::Bouncer:
			Shift_Renderer_2->SPRITE(L"equip_icon_weapon_bouncer.png");
			break;
		case Inventory_Bullet_Type::Charge:
			Shift_Renderer_2->SPRITE(L"equip_icon_weapon_charge.png");
			break;
		default:
			Shift_Renderer_2->SPRITE(L"equip_icon_empty.png");
			break;
		}

	}

	Debug();
}

void Scene_WorldMap::SceneChangeStart()
{
	//카메라 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"ACTORCAMERA");
		PTR->TRANS()->WPOS({ 640.f,  -850.f, -100.f });
		Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(0,
			(int)RENDERORDER::RENDERORDER_ACTOR,
			(int)RENDERORDER::RENDERORDER_MAP,
			(int)RENDERORDER::RENDERORDER_Debug);
		Cam->CAMSIZE({ 1280.f, 720.f });
		Cam->MODE(Cam_Mode::ORTH);

		//두번째부터는 위치 저장.
		Game_Vector Def_Vector = { -99999.f, -99999.f, -99999.f };
		if (Player_Map::Player_Pos != Def_Vector)
			PTR->TRANS()->WPOS({ Player_Map::Player_Pos.X, Player_Map::Player_Pos.Y, -100.f });
	}

	//노이즈
	{

		{
			//카메라 생성
			Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"NoiseCam");
			PTR->TRANS()->WPOS({ 0.f,  0.f, -100.f });
			Game_Ptr<Game_Cam> Cam = PTR->CreateCom<Game_Cam>(2,
				(int)RENDERORDER::RENDERORDER_Noise);
			Cam->CAMSIZE({ 1024.f, 512.f });
			Cam->MODE(Cam_Mode::ORTH);
			}
		{

		//노이즈 생성
		Noise = SCENE()->CreateActor(L"Noise");
		Noise->TRANS()->WSCALE({ 1024.f, 512.f, 1.0f });
		Noise->TRANS()->WPOS({ 0.f, 0.f, -50.f });
		Noise_Renderer = Noise->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
		Noise_Ani = Noise->CreateCom<Game_Animation>(Noise_Renderer);
		Noise_Ani->CreateAni(L"Noise", L"Noise1.png", 0, 125, 0.1f, true);
		Noise_Ani->ChangeAni(L"Noise");
		}
	}

	//Shift
	{
		Shift = SCENE()->CreateActor();
		Shift->TRANS()->WSCALE({ 600.0f, 600.0f, 1.0f });
		Shift->TRANS()->WPOS({ 0.f, 0.f, -97.f });
		Shift_Renderer = Shift->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
		Shift_Renderer->SPRITE(L"Shift.png");
		Shift_Renderer->Off();

		Shift_1 = SCENE()->CreateActor();
		Shift_1->TRANS()->WSCALE(Shift_Icon_Size);
		Shift_1->TRANS()->WPOS({ -140.f, Shift_Icon_LPos_Y - 2.f, -98.f });
		Shift_Renderer_1 = Shift_1->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
		Shift_Renderer_1->SPRITE(L"equip_icon_weapon_bouncer.png");
		Shift_Renderer_1->Off();

		Shift_2 = SCENE()->CreateActor();
		Shift_2->TRANS()->WSCALE(Shift_Icon_Size);
		Shift_2->TRANS()->WPOS({ -12.f, Shift_Icon_LPos_Y - 8.f, -98.f });
		Shift_Renderer_2 = Shift_2->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
		Shift_Renderer_2->SPRITE(L"equip_icon_weapon_bouncer.png");
		Shift_Renderer_2->Off();

		Shift_3 = SCENE()->CreateActor();
		Shift_3->TRANS()->WSCALE(Shift_Icon_Size);
		Shift_3->TRANS()->WPOS({ 118.f, Shift_Icon_LPos_Y - 10.f, -98.f });
		Shift_Renderer_3 = Shift_3->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
		Shift_Renderer_3->SPRITE(L"equip_icon_weapon_bouncer.png");
		Shift_Renderer_3->Off();
	}

	//플레이어 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Player_Map");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 550.f, -850.f, 1.0f });

		PTR->CreateCom<Player_Map>();

		//두번째부터는 위치 저장.
		Game_Vector Def_Vector = { -99999.f, -99999.f, -99999.f };
		if (Player_Map::Player_Pos != Def_Vector)
			PTR->TRANS()->WPOS(Player_Map::Player_Pos);
	}


	//배경 생성
	{
		//배경
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"WorldMap1.png");
		PTR->TRANS()->WSCALE({ 3712.f, 2000.f, 1.0f });
		PTR->TRANS()->WPOS({ +1856.f, -1000.f, 30.0F });
		Game_Ptr<Game_Sprite_Renderer> RENDER = PTR->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		RENDER->SPRITE(L"WorldMap1.png");
		
		{
		//윗배경
		Background_Up = SCENE()->CreateActor(L"WorldMap1_Top.png");
		Background_Up->TRANS()->WSCALE({ 3712.f, 2000.f, 1.0f });
		Background_Up->TRANS()->WPOS({ +1856.f, -1000.f, 0.f });
		Background_Up_Renderer = Background_Up->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
		Background_Up_Renderer->SPRITE(L"WorldMap1_Top.png");
		}
	}

	//입구 생성
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Slime");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 2000.f, -610.f, 10.0F });
		Game_Ptr<Map_Entrance> Entrance = PTR->CreateCom<Map_Entrance>();
		Entrance->Set_Type(Map_Type::Slime);
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Monkey");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 3121.f, -281.f, 10.0F });
		Game_Ptr<Map_Entrance> Entrance = PTR->CreateCom<Map_Entrance>();
		Entrance->Set_Type(Map_Type::Monkey);
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Ghost");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 2975.f, -1669.f, 10.0F });
		Game_Ptr<Map_Entrance> Entrance = PTR->CreateCom<Map_Entrance>();
		Entrance->Set_Type(Map_Type::Ghost);
	}
	{
		Game_Ptr<Game_Actor> PTR = SCENE()->CreateActor(L"Shop");
		PTR->TRANS()->WSCALE({ 1.f, 1.f, 1.f });
		PTR->TRANS()->WPOS({ 1990.f, -1110.f, 1.0F });
		Game_Ptr<Map_Entrance> Entrance = PTR->CreateCom<Map_Entrance>();
		Entrance->Set_Type(Map_Type::Shop);
	}

}

void Scene_WorldMap::SceneChangeEnd()
{
	SCENE()->ActorClear();
}

void Scene_WorldMap::Debug()
{
	//디버그
	{
		if (Game_Input::Down(L"DebugCheck"))
		{
			if (m_DebugCheck)
				m_DebugCheck = false;
			else
				m_DebugCheck = true;
		}

		if (Game_Input::Down(L"DebugOn"))
		{
			m_DebugCheck = true;
		}
		else if (Game_Input::Down(L"DebugOff"))
		{
			m_DebugCheck = false;
		}

		if (m_DebugCheck)
		{
			Game_3D_Debug::DrawDebugText(L"Up z : %f", Background_Up->TRANS()->WPOS().Z);
			Game_3D_Debug::DrawDebugText(L"Noise z : %f", Noise->TRANS()->WPOS().Z);
		}
	}
}
