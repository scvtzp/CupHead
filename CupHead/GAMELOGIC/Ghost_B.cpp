#include "Ghost_B.h"
#include "Game_FreeCam.h"
#include "LOGIC_Value.h"
#include "Game_Header.h"
#include "Logic_Enum.h"

#define Ghost_B_Size_Default {400.f, 400.f, 1.f}
#define Ghost_B_Collision_Scale_Default {100.f, 100.f, 1.f}
#define Ghost_B_Collision_LPos_Default { 0.0F, +50.0F, -1.0F }

void Ghost_B::Init()
{
	m_Hp = 1;

	m_Gravity = 0.5F;
	m_Speed = 100.0F;
	m_Cam = SCENE()->MainCam();

	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Monster_Parry);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Ghost_B_Collision_Scale_Default);
		m_Collision->LPOS(Ghost_B_Collision_LPos_Default);

		m_Collision_Debug_Render = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_Debug);
		m_Collision_Debug_Render->LSCALE(Ghost_B_Collision_Scale_Default);
		m_Collision_Debug_Render->LPOS(Ghost_B_Collision_LPos_Default);
		m_Collision_Debug_Render->SPRITE(L"Col.png");
	}

	Renderer_Setting();

	m_Gradient = (TRANS()->LPOS().Y / TRANS()->LPOS().X);
}

void Ghost_B::Update()
{
	Move();
}

void Ghost_B::Hit(Game_Collision* _This, Game_Collision* _Other)
{
}

void Ghost_B::Renderer_Setting()
{
	m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
	m_Animation->LSCALE(Ghost_B_Size_Default);

	m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);

	m_Animation_Type->CreateAni(L"Idle", L"Ghost_B.png", 0, 23, 0.1f, true);

	m_Animation_Type->ChangeAni(L"Idle");
}

void Ghost_B::Move()
{
	if (TRANS()->LPOS().X > 0.f)
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
	else if (TRANS()->LPOS().X < 0.f)
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}

	TRANS()->LPOS({ TRANS()->LPOS().X , (TRANS()->LPOS().X * m_Gradient)-120.f, TRANS()->LPOS().Z });
}
