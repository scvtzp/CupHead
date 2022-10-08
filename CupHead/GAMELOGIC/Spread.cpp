#include "Spread.h"
#include "Logic_Enum.h"
#include "Game_Header.h"

#define Spread_Col_Size {25.f, 25.f, 1.f}
#define Spread_Col_LPos {0.f, 0.f, -1.f}

void Spread::Init()
{
	m_Speed = 400.0F;
	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::COLORDER_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Spread_Col_Size);
		m_Collision->LPOS(Spread_Col_LPos);
		m_Collision->PushEnterFunc(this, &Spread::Hit);

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

		m_Animation_Type->CreateAni(L"Death", L"Spread.png", 0, 3, 0.05f, false);
		m_Animation_Type->CreateAni(L"Hit", L"Spread.png", 4, 9, 0.05f, false);
		m_Animation_Type->CreateAni(L"Main", L"Spread.png", 10, 13, 0.05f, true);
		m_Animation_Type->CreateAni(L"Spark", L"Spread.png", 14, 17, 0.05f, false);

		m_Animation_Type->ChangeAni(L"Spark");
	}
}

void Spread::Update()
{
	Debug();
	Spread_LifeTime += Game_Time::DeltaTime();

	if (m_Animation_Type->Get_Ani() == L"Spark")
	{
		if (m_Animation_Type->Get_Num() == m_Animation_Type->Get_EndNum())
		{
			m_Animation_Type->ChangeAni(L"Main");
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
	else if(Spread_LifeTime >= 0.7f)
	{
		Bullet_Death();
		return;
	}
	Move();
}

void Spread::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	DeathCheck = true;
}

void Spread::Move()
{
	////좌하
	//else if (TRANS()->WROT().Z == 225.f)
	//{
		//TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
	//	TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	//}
		

	float X(0), Y(0);
	X = cosf(TRANS()->WROT().Z * PI / 180.f) * Game_Time::DeltaTime(m_Speed);
	Y = sinf(TRANS()->WROT().Z * PI / 180.f) * Game_Time::DeltaTime(m_Speed);
	TRANS()->WPOS({ TRANS()->WPOS().X + X , TRANS()->WPOS().Y +Y ,TRANS()->WPOS().Z});
}

void Spread::Debug()
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

void Spread::Bullet_Death()
{
	if (DeathCheck)
		m_Animation_Type->ChangeAni(L"Hit");
	else
		m_Animation_Type->ChangeAni(L"Death");

	if (m_Animation_Type->Get_Num() >= m_Animation_Type->Get_EndNum())
	{
		ACTOR()->Death();
		m_Collision_Debug_Render->Death();
	}
}
