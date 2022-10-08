#include "Logic_Enum.h"
#include "Charge.h"
#include "Logic_Enum.h"
#include "Game_Header.h"

#define Charge_Col_Size {40.f, 40.f, 1.f}
#define Charge_Col_LPos {0.f, 0.f, -1.f}

void Charge::Init()
{
	m_Speed = 400.0F;
	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Charge_Col_Size);
		m_Collision->LPOS(Charge_Col_LPos);
		m_Collision->PushEnterFunc(this, &Charge::Hit);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
		m_Collision_Debug_Render->LPOS(m_Collision->LPOS());
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE({ 400.f, 400.f, 1.0f });

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Intro", L"Charge_Large.png", 0, 2, 0.05f, false);
		m_Animation_Type->CreateAni(L"Idle", L"Charge_Large.png", 3, 14, 0.05f, true);
		m_Animation_Type->CreateAni(L"Death", L"Charge_Large.png", 21, 25, 0.05f, false);

		m_Animation_Type->CreateAni(L"Idle_S", L"Charge_Small.png", 11, 18, 0.05f, true);
		m_Animation_Type->CreateAni(L"Death_S", L"Charge_Small.png", 6, 10, 0.05f, false);

		m_Animation_Type->CreateAni(L"Spark", L"Charge_Small.png", 19, 22, 0.05f, false);

		m_Animation_Type->ChangeAni(L"Spark");
	}
}

void Charge::Update()
{
	Debug();

	if (m_Animation_Type->Get_Ani() == L"Spark")
	{
		if (m_Animation_Type->Get_Num() == m_Animation_Type->Get_EndNum())
		{
			if(ChargeType)
				m_Animation_Type->ChangeAni(L"Intro");
			else
				m_Animation_Type->ChangeAni(L"Idle_S");
		}
	}

	if (m_Animation_Type->Get_Ani() == L"Intro")
	{
		if (m_Animation_Type->Get_Num() == m_Animation_Type->Get_EndNum())
		{
			m_Animation_Type->ChangeAni(L"Idle");
		}
	}


	//벽닿으면 삭제
	if (TRANS()->WPOS().X <= -640.f ||
		TRANS()->WPOS().X >= 640.f ||
		TRANS()->WPOS().Y <= -360.f ||
		TRANS()->WPOS().Y >= 360.f)
	{
		Bullet_Death();
		return;
	}
	else if (DeathCheck) //맞아도 삭제
	{
		Bullet_Death();
		return;
	}
	Move();
}

void Charge::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	DeathCheck = true;
}

void Charge::Move()
{
	float X(0), Y(0);
	X = cosf(TRANS()->WROT().Z * PI / 180.f) * Game_Time::DeltaTime(m_Speed);
	Y = sinf(TRANS()->WROT().Z * PI / 180.f) * Game_Time::DeltaTime(m_Speed);
	TRANS()->WPOS({ TRANS()->WPOS().X + X , TRANS()->WPOS().Y + Y ,TRANS()->WPOS().Z });
}

void Charge::Debug()
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

		if (m_DebugCheck)
		{
			m_Collision_Debug_Render->On();
		}
		else
		{
			m_Collision_Debug_Render->Off();
		}
	}
}

void Charge::Bullet_Death()
{
	if (ChargeType)
		m_Animation_Type->ChangeAni(L"Death");
	else
		m_Animation_Type->ChangeAni(L"Death_S");

	if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum())
	{
		ACTOR()->Death();
		m_Collision_Debug_Render->Death();
	}
}
