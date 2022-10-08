#include "Sky_PeaShot.h"
#include "Logic_Enum.h"

#define Sky_Bomb_Collision_Size {30.f, 30.f, 1.f}
#define Sky_Bomb_Collision_LPos {0.f, 0.f, -1.f}

int Sky_PeaShot::Count = 0;

void Sky_PeaShot::Init()
{
	m_Speed = 800.0F;

	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Sky_Bomb_Collision_Size);
		m_Collision->LPOS(Sky_Bomb_Collision_LPos);
		m_Collision->PushEnterFunc(this, &Sky_PeaShot::Hit);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Sky_Bomb_Collision_Size);
		m_Collision_Debug_Render->LPOS(Sky_Bomb_Collision_LPos);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE({ 128.f, 128.f, 1.0f });

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"A", L"Sky_PeaShot.png", 0, 2, 0.15f, true);
		m_Animation_Type->CreateAni(L"B", L"Sky_PeaShot.png", 3, 5, 0.15f, true);
		m_Animation_Type->CreateAni(L"C", L"Sky_PeaShot.png", 6, 8, 0.15f, true);

		switch (Count)
		{
		case 0:
			m_Animation_Type->ChangeAni(L"A");
			break;
		case 1:
			m_Animation_Type->ChangeAni(L"B");
			TRANS()->Set_LPosY(TRANS()->LPOS().Y + 7.f);
			break;
		case 2:
			m_Animation_Type->ChangeAni(L"C");
			TRANS()->Set_LPosY(TRANS()->LPOS().Y - 7.f);
			break;
		default:
			break;
		}

		Count++;
		if (Count >= 3)
			Count = 0;
	}
}

void Sky_PeaShot::Update()
{
	Debug();
	Move();

	//벽닿으면 삭제
	if (TRANS()->WPOS().X <= -640.f ||
		TRANS()->WPOS().X >= 640.f ||
		TRANS()->WPOS().Y <= -360.f ||
		TRANS()->WPOS().Y >= 360.f)
	{
		ACTOR()->Death();
		m_Collision_Debug_Render->Death();
	}
}

void Sky_PeaShot::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	ACTOR()->Death();
	m_Collision_Debug_Render->Death();
}

void Sky_PeaShot::Move()
{
	TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
}

void Sky_PeaShot::Debug()
{
	//디버그
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
		//Game_3D_Debug::DrawDebugText(L"Player Local Pos : %f, %f", TRANS()->LPOS().X, TRANS()->LPOS().Y);
		//Game_3D_Debug::DrawDebugText(L"Player Hp : %d", m_Hp);
	}
}
