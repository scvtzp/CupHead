#pragma once
#include "HGAMEPLAYER.h"

enum class Slime_Animation_Type
{
	Mn_Jump,
	Mn_,
	End
};

enum class Slime_State
{
	Mn_Intro,
	Mn_Jump,
	Mn_Atk,
	Mn_Trans,

	Lg_Jump,
	Lg_Atk,
	Lg_Trans,

	To_Move,
	To_Atk,
	To_Dead,
	
	End
};

enum class Slime_Phase
{
	Mini,
	Big,
	Tomb
};


class Slime : public MOVECOM
{
private:
	Slime_State m_eState = Slime_State::Mn_Intro;

	Slime_Animation_Type Past_AnimationType = Slime_Animation_Type::End;
	Slime_Animation_Type Present_AnimationType = Slime_Animation_Type::End;
	Slime_Phase m_Phase = Slime_Phase::Mini;
	
	Game_Ptr<Game_Sprite_Renderer>	m_Effect_Animation;
	Game_Ptr<Game_Animation>		m_Effect_Animation_Type;

	Game_Ptr<Game_Collision> m_Collision_ForTo;

	bool m_bLeft = true;
	bool m_bCheck = false;

	bool m_JumpCheck = false;
	float TimeCount = 0.f;
	float TimeCount_Death = 0.f;
	int To_HitCheck = 0;

public:
	void Init() override;
	void Update() override;
	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Renderer_Setting();

public:
	void DeathCheck();
	void Move();
	void Jump();
	void PunchCheck();

	void Mn_Intro();
	void Mn_Trans();
	void Punch();

	void Lg_Punch();
	void Lg_Trans();

	void To_AtkCheck(Game_Collision* _This, Game_Collision* _Other);
	void To_AtkCheck();
	void To_Atk();
	void To_Death();
	void To_Move();
};