#include "Sky_Bomb.h"
#include "Logic_Enum.h"

#define Sky_Bomb_Collision_Size {50.f, 50.f, 1.f}
#define Sky_Bomb_Collision_LPos {0.f, 0.f, -1.f}

void Sky_Bomb::Init()
{
	m_Speed = 600.0F;
	m_SpeedY = 200.f;

	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Sky_Bomb_Collision_Size);
		m_Collision->LPOS(Sky_Bomb_Collision_LPos);
		m_Collision->PushEnterFunc(this, &Sky_Bomb::Hit);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Sky_Bomb_Collision_Size);
		m_Collision_Debug_Render->LPOS(Sky_Bomb_Collision_LPos);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE({ 400.f, 400.f, 1.0f });

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Idle", L"Sky_Bomb.png", 0, 6, 0.15f, false);

		m_Animation_Type->ChangeAni(L"Idle");
	}
}

void Sky_Bomb::Update()
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

void Sky_Bomb::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	ACTOR()->Death();
	m_Collision_Debug_Render->Death();

}

void Sky_Bomb::Move()
{
	int r = 400;
	TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_SpeedY));
	m_SpeedY += 10.f;
}

void Sky_Bomb::Debug()
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
