#include "Monkey_Bullet.h"
#define Monkey_Bullet_Collision_Size  { 80.f, 80.f, 1.f }
#define Monkey_Bullet_Collision_LPos  { 0.f, 0.f, 0.f }

#define Monkey_Bullet_Size { 200.f, 200.f, 1.0f }
void Monkey_Bullet::Init()
{
	m_Speed = 400.0F;
	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Monster_Bullet);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Monkey_Bullet_Collision_Size);
		m_Collision->LPOS(Monkey_Bullet_Collision_LPos);
		m_Collision->PushEnterFunc(this, &Monkey_Bullet::Hit);

		//충돌 범위 테스트용 출력
		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Monkey_Bullet_Collision_Size);
		m_Collision_Debug_Render->LPOS(Monkey_Bullet_Collision_LPos);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	{
		m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		m_Animation->LSCALE(Monkey_Bullet_Size);

		m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

		m_Animation_Type->CreateAni(L"Shot", L"Monkey_MusicNotes.png", 0, 16, 0.2f, false);

		m_Animation_Type->ChangeAni(L"Shot");
	}
}

void Monkey_Bullet::Update()
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

void Monkey_Bullet::Hit(Game_Collision* _This, Game_Collision* _Other)
{
	ACTOR()->Death();
	m_Collision_Debug_Render->Death();
}

void Monkey_Bullet::Move()
{
	//우
	if (TRANS()->WROT().Z == 0.f)
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
	//좌
	else if (TRANS()->WROT().Z == 180.f)
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
	//상
	else if (TRANS()->WROT().Z == 90.f)
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
	}
	//하
	else if (TRANS()->WROT().Z == 270.f)
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
	}
	//우상
	else if (TRANS()->WROT().Z == 45.f)
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
	//우하
	else if (TRANS()->WROT().Z == 315.f)
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
	//좌상
	else if (TRANS()->WROT().Z == 135.f)
	{
		TRANS()->WMOVE(Game_Vector::UP * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
	//좌하
	else if (TRANS()->WROT().Z == 225.f)
	{
		TRANS()->WMOVE(Game_Vector::DOWN * Game_Time::DeltaTime(m_Speed));
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
}

void Monkey_Bullet::Debug()
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