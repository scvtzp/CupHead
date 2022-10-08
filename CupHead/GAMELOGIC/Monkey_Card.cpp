#include "Monkey_Card.h"
#include <Game_3D_Debug.h>
#include "Logic_Enum.h"

#define Monkey_Card_Size_Back {400.f, 400.f, 1.f}
#define Monkey_Card_Size {240.f, 170.f, 1.f}
#define Monkey_Card_Size_X 240.f
#define Dot_Size {30.f, 30.f, 1.f}
#define Dot_LPos {0.f, 0.f, -1.f }

Monkey_CardType Monkey_Card::Card_1 = Monkey_CardType::End;
Monkey_CardType Monkey_Card::Card_2 = Monkey_CardType::End;

void Monkey_Card::Init()
{
	//面倒
	{
		m_Collision = ACTOR()->CreateCom<Game_Collision>((int)COLORDER::Monster_Parry);
		m_Collision->ColType(COLTYPE::AABB2D);
		m_Collision->LSCALE(Dot_Size);
		m_Collision->LPOS(Dot_LPos);
	}

	// Dot 积己
	{
		Dot_Renderer = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
		Dot_Renderer->LSCALE(Dot_Size);
		Dot_Renderer->LPOS(Dot_LPos);
		Dot_Renderer->SPRITE(L"Card_Dot.png");
	}
	Renderer_Setting();
}

void Monkey_Card::Update()
{
	Renderer_Update();

	CardType = CardType;

	if (ParryCheck)
	{
		CardType = Monkey_CardType::End;
	}
}

void Monkey_Card::Hit(Game_Collision* _This, Game_Collision* _Other)
{
}

void Monkey_Card::Renderer_Setting()
{
	m_Animation = ACTOR()->CreateCom<Game_Sprite_Renderer>((int)RENDERORDER::RENDERORDER_ACTOR);
	m_Animation->LSCALE(Monkey_Card_Size_Back);

	m_Animation_Type = ACTOR()->CreateCom<Game_Animation>(m_Animation);
	m_Animation_Type->CreateAni(L"Back", L"Monkey_Card_Back.png", 0, 11, 0.1f, true);
	m_Animation_Type->CreateAni(L"Monkey_Card_Cuphead", L"Monkey_Card_Cuphead.png", 0, 0, 0.1f, false);
	m_Animation_Type->CreateAni(L"Monkey_Card_Mugman", L"Monkey_Card_Mugman.png", 0, 0, 0.1f, false);
	m_Animation_Type->CreateAni(L"Monkey_Card_Chips", L"Monkey_Card_Chips.png", 0, 0, 0.1f, false);
	m_Animation_Type->CreateAni(L"Monkey_Card_Flowers", L"Monkey_Card_Flowers.png", 0, 0, 0.1f, false);
	m_Animation_Type->CreateAni(L"Monkey_Card_Shield", L"Monkey_Card_Shield.png", 0, 0, 0.1f, false);
	m_Animation_Type->CreateAni(L"Monkey_Card_Spindle", L"Monkey_Card_Spindle.png", 0, 0, 0.1f, false);
	m_Animation_Type->ChangeAni(L"Back");
}

void Monkey_Card::Renderer_Update()
{ 
	//Dot 眉农 犬牢.
	if (m_Collision->Get_Check())
	{
		if (!Change_Check)
		{
			m_Animation->LSCALE(Monkey_Card_Size);
			Effect_On = true;
		}

 

		Change_Check = true;
		Dot_Renderer->Off();
		Effect();
	}
	else
	{
		if (Change_Check)
		{
			Change_Check = false;
			Effect_On = true;
		}

		Effect();

		if (!Effect_On)
		{
			m_Animation->LSCALE(Monkey_Card_Size_Back);
			m_Animation_Type->ChangeAni(L"Back");
			Dot_Renderer->On();
		}
		return;
	}

	switch (CardType)
	{
	case Monkey_CardType::Cuphead:
		m_Animation_Type->ChangeAni(L"Monkey_Card_Cuphead");
		break;
	case Monkey_CardType::MugMan:
		m_Animation_Type->ChangeAni(L"Monkey_Card_Mugman");
		break;
	case Monkey_CardType::Chips:
		m_Animation_Type->ChangeAni(L"Monkey_Card_Chips");
		break;
	case Monkey_CardType::Flowers:
		m_Animation_Type->ChangeAni(L"Monkey_Card_Flowers");
		break;
	case Monkey_CardType::Shield:
		m_Animation_Type->ChangeAni(L"Monkey_Card_Shield");
		break;
	case Monkey_CardType::Spindle:
		m_Animation_Type->ChangeAni(L"Monkey_Card_Spindle");
		break;
	default:
		m_Animation->LSCALE(Monkey_Card_Size_Back);
		m_Animation_Type->ChangeAni(L"Back");
		break;
	}

	if (CardType != Monkey_CardType::End && !Effect_On)
	{
		m_Animation->LSCALE(Monkey_Card_Size);
	}
}

void Monkey_Card::Effect()
{
	if (!Effect_On)
	{
		m_Animation->Set_LScaleX(m_Animation->LSCALE().X + 10.f);
		if (m_Animation->LSCALE().X >= Monkey_Card_Size_X)
			m_Animation->Set_LScaleX(Monkey_Card_Size_X);
		return;
	}

	Effect_Time += Game_Time::DeltaTime();

	if (Effect_Time >= 0.7f)
	{
		Effect_On = false;
		Effect_Time = 0.f;
	}
	else
	{
		//m_Collision->Check();
	}


	if (Effect_Check)
	{
		m_Animation->Set_LScaleX(m_Animation->LSCALE().X + 10.f);
	}
	else if(!Effect_Check)
	{
		m_Animation->Set_LScaleX(m_Animation->LSCALE().X - 10.f);
	}

	if (m_Animation->LSCALE().X <= 10.f)
	{
		Effect_Check = true;
	}
	else if (m_Animation->LSCALE().X >= Monkey_Card_Size_X)
	{
		Effect_Check = false;
	}
}
