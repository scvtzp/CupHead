#include "Bouncer.h"
#include "Logic_Enum.h"
#include "Game_Header.h"

#define Bouncer_Col_Size {25.f, 25.f, 1.f}
#define Bouncer_Col_LPos {0.f, 0.f, -1.f}
#define Bouncer_Col_Size_Y 25.f
void Bouncer::Init()
{
	m_Speed = 300.0F;
	m_Cam = SCENE()->MainCam();

	GRAVITYVEC = { 0,200.0F,0 };
	JUMPVEC = { 0,70.0F,0 };

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Bouncer_Col_Size);
		m_Collision->LPOS(Bouncer_Col_LPos);
		m_Collision->PushEnterFunc(this, &Bouncer::Hit);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(m_Collision->LSCALE());
		m_Collision_Debug_Render->LPOS(m_Collision->LPOS());
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE({ 200.f, 200.f, 1.0f });

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Bounce", L"Bouncer.png", 0, 2, 0.08f, false);
		m_Animation_Type->CreateAni(L"Hit", L"Bouncer.png", 3, 8, 0.05f, false);
		m_Animation_Type->CreateAni(L"Main", L"Bouncer.png", 9, 19, 0.05f, true);
		m_Animation_Type->CreateAni(L"Spark", L"Bouncer.png", 20, 23, 0.05f, false);

		m_Animation_Type->ChangeAni(L"Spark");
	}
}

void Bouncer::Update()
{
	Debug();

	if (m_Animation_Type->Get_Ani() == L"Spark" || m_Animation_Type->Get_Ani() == L"Bounce")
	{
		if (m_Animation_Type->Get_Num() == m_Animation_Type->Get_EndNum())
		{
			m_Animation_Type->ChangeAni(L"Main");
		}
	}

	else
	{
		Move();
	}

	//벽닿으면 삭제
	if (TRANS()->WPOS().X <= -640.f ||
		TRANS()->WPOS().X >= 640.f)
	{
		Bullet_Death();
		return;
	}
	else if (DeathCheck) //맞아도 삭제
	{
		Bullet_Death();
		return;
	}
}

void Bouncer::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	DeathCheck = true;
}

void Bouncer::Move()
{
	float X(0), Y(0);
	X = cosf(TRANS()->WROT().Z * PI / 180.f) * Game_Time::DeltaTime(m_Speed);
	TRANS()->WPOS({ TRANS()->WPOS().X + X , TRANS()->WPOS().Y,TRANS()->WPOS().Z });

	TRANS()->WMOVE(JUMPVEC * Game_Time::DeltaTime());
	JUMPVEC -= GRAVITYVEC * Game_Time::DeltaTime();

	if (TRANS()->LPOS().Y < Floor + (Bouncer_Col_Size_Y / 2) - Bouncer_Col_Size_Y)
	{
		GRAVITYVEC -= { 0,10.f,0 };
		JUMPVEC = { 0,100.0f,0 };
		m_Animation_Type->ChangeAni(L"Bounce");

	}

}

void Bouncer::Debug()
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

void Bouncer::Bullet_Death()
{
	if (DeathCheck)
		m_Animation_Type->ChangeAni(L"Hit");
	else
		m_Animation_Type->ChangeAni(L"Hit");

	if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum())
	{
		ACTOR()->Death();
		m_Collision_Debug_Render->Death();
	}
}
