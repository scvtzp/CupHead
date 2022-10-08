#pragma once
#include "Game_Logic.h"

class MOVECOM : public Game_Actor_Com
{
public:
	Game_Ptr<Game_Cam> m_Cam;

	Game_Vector JUMPVEC;
	Game_Vector GRAVITYVEC;

	Game_Ptr<Game_Sprite_Renderer>	m_Animation;
	Game_Ptr<Game_Animation>		m_Animation_Type;

	Game_Ptr<Game_Actor> m_Collision_Debug;
	Game_Ptr<Game_Actor> m_Collision_1_Debug;

	Game_Ptr<Game_Collision> m_Collision;
	Game_Ptr<Game_Collision> m_Collision_1;

	Game_Ptr<Game_Sprite_Renderer> m_Collision_Debug_Render;
	Game_Ptr<Game_Sprite_Renderer> m_Collision_1_Debug_Render;

	int m_Hp = 3;
	float m_Power = 0;

	float m_Speed;
	float m_Gravity;

	bool m_DebugCheck = true;

	void Set_Hp(int Hp) { m_Hp -= Hp; }

public:
	void Synchro_Collistion()
	{
		m_Collision_1->LSCALE(m_Collision->LSCALE());
		m_Collision_1->LPOS(m_Collision->LPOS());

		//m_Collision_Debug->TRANS()->WSCALE(m_Collision->WSCALE());
		//m_Collision_Debug->TRANS()->WPOS({ m_Collision->WPOS().X,m_Collision->WPOS().Y, -2.f });
	}

	bool Animation_Check(const Game_String& _Name, int _Num)
	{
		if (m_Animation_Type->Get_Ani() == _Name)
		{
			if (m_Animation_Type->Get_Num() >= _Num)
			{
				return true;
			}
		}
		
		return false;
	}

	bool Animation_Check(const Game_String& _Name, int _Num1, int _Num2)
	{
		if (m_Animation_Type->Get_Ani() == _Name)
		{
			if (m_Animation_Type->Get_Num() >= _Num1 && m_Animation_Type->Get_Num() <= _Num2)
			{
				return true;
			}
		}

		return false;
	}
};








enum class PLAYERSTATE
{
	PS_IDLE,
	PS_WALK,
	PS_JUMP,
};

class HGAMEPLAYER : public MOVECOM
{
private:
	Game_Ptr<Game_Cam> m_Cam;
	Game_Vector JUMPVEC;
	Game_Vector GRAVITYVEC;
	PLAYERSTATE m_eState;

public:
	void KeyMove();
	void JumpCheck();

public:
	void IDLE();
	void WALK();
	void JUMP();

public:
	void Init() override;
	void Update() override;
};

