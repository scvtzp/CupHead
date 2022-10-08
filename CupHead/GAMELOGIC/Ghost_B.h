#pragma once
#include "HGAMEPLAYER.h"

class Ghost_B : public MOVECOM
{
private:
	//Slime_State m_eState = Slime_State::Mn_Intro;

	//Slime_Animation_Type Past_AnimationType = Slime_Animation_Type::End;
	//Slime_Animation_Type Present_AnimationType = Slime_Animation_Type::End;
	//Slime_Phase m_Phase = Slime_Phase::Mini;

	Game_Ptr<Game_Sprite_Renderer>	m_Effect_Animation;
	Game_Ptr<Game_Animation>		m_Effect_Animation_Type;

	Game_Ptr<Game_Collision> m_Collision_ForTo;

	bool m_bLeft = true;
	bool m_bCheck = false;

	float m_Gradient = 0.f; //±â¿ï±â

	bool m_JumpCheck = false;
	float TimeCount = 0.f;
	float TimeCount_Death = 0.f;
	int To_HitCheck = 0;

public:
	void Init() override;
	void Update() override;
	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Renderer_Setting();

	void Move();

};

