#include "Map_Entrance.h"
#include "Logic_Enum.h"
#include "Player_Map.h"
#include <Game_3D_Debug.h>

#define MapEntrance_Size {200.f, 200.f, 100.f}
#define MapEntrance_LPos {0.f, 0.f, 0.f}

void Map_Entrance::Init()
{
	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Map_Entrance);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(MapEntrance_Size);
		m_Collision->LPOS(MapEntrance_LPos);
		m_Collision->PushEnterFunc(this, &Map_Entrance::Hit);
		m_Collision->PushStayFunc(this, &Map_Entrance::Hit);
		m_Collision->PushExitFunc(this, &Map_Entrance::Out);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(MapEntrance_Size);
		m_Collision_Debug_Render->LPOS(MapEntrance_LPos);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	//간판
	Sign_Renderer = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Noise);
	Sign_Renderer->LSCALE({400.f, 250.f, 1.f});
	Sign_Renderer->LPOS({ -TRANS()->WPOS().X,-TRANS()->WPOS().Y, -20.f });
	Sign_Renderer->SPRITE(L"Col.png");
	Sign_Renderer->Off();

	Renderer_Setting();
}

void Map_Entrance::Update()
{
	switch (MapType)
	{
	case Map_Type::Slime:
		m_Animation_Type->ChangeAni(L"Slime");
		break;
	case Map_Type::Monkey:
		m_Animation_Type->ChangeAni(L"Monkey");
		break;
	case Map_Type::Ghost:
		m_Animation_Type->ChangeAni(L"Ghost");
		break;
	case Map_Type::Shop:
		m_Animation_Type->ChangeAni(L"Shop");
		break;
	case Map_Type::End:
		break;
	default:
		break;
	}

	Debug();
}

void Map_Entrance::Renderer_Setting()
{
	m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_MAP);
	m_Animation->LSCALE({400.f, 400.f, 1.f});
	m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

	m_Animation_Type->CreateAni(L"Monkey", L"World1_Icon.png", 0, 2, 0.1f, true);
	m_Animation_Type->CreateAni(L"Ghost", L"World1_Icon.png", 3, 5, 0.1f, true);
	m_Animation_Type->CreateAni(L"Shop", L"World1_Icon.png", 9, 11, 0.1f, true);
	m_Animation_Type->CreateAni(L"Slime", L"World1_Icon.png", 12, 14, 0.1f, true);

	m_Animation_Type->ChangeAni(L"Shop");
}

void Map_Entrance::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	if (Player_Map::Get_Entrance_Check())
	{
		switch (MapType)
		{
		case Map_Type::Slime:
			Game_Scene::ChangeScene(L"Stage_Slime");
			break;
		case Map_Type::Monkey:
			Game_Scene::ChangeScene(L"Stage_Monkey");
			break;
		case Map_Type::Ghost:
			Game_Scene::ChangeScene(L"Stage_Ghost");
			break;
		case Map_Type::Shop:
			Game_Scene::ChangeScene(L"Shop");
			break;
		case Map_Type::End:
			break;
		default:
			break;
		}
	}
	else
	{
		switch (MapType)
		{
		case Map_Type::Slime:
			Sign_Renderer->SPRITE(L"Slime_Signboard.png");
			break;
		case Map_Type::Monkey:
			Sign_Renderer->SPRITE(L"Monkey_Sign.PNG");
			break;
		case Map_Type::Ghost:
			Sign_Renderer->SPRITE(L"Ghost_Sign.PNG");
			break;
		case Map_Type::Shop:
			Sign_Renderer->SPRITE(L"Shop_Sign.PNG");
			break;
		default:
			Sign_Renderer->SPRITE(L"Shop_Sign.PNG");
			break;
		}

		Sign_Renderer->On();
	}
}

void Map_Entrance::Out(Game_Collision* _This, Game_Collision* _Other)
{
	Sign_Renderer->Off();
}

void Map_Entrance::Set_Type(Map_Type type)
{
	MapType = type;
}

void Map_Entrance::Debug()
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
			m_Collision_Debug_Render->On();
		}
		else if (Game_Input::Down(L"DebugOff"))
		{
			m_DebugCheck = false;
			m_Collision_Debug_Render->Off();
		}

		if (m_DebugCheck)
		{

		}
	}

}
