#pragma once
#include "HGAMEPLAYER.h"
enum class Player_Sky_State
{
	Intro,
	
	Idle_Up,
	Idle_Down,
	Idle_Straight,

	Trans_Up,
	Trans_Down,
	
	Parry,

	Trans_Mn,
	Trans_Lg,

	Mn_Idle_Up,
	Mn_Idle_Down,
	Mn_Idle_Straight,

	Mn_Trans_Up,
	Mn_Trans_Down,

	Ghost,

	End
};

enum class Player_Sky_ShootType
{
	Bomb,
	PeaShot,
	End
};

class Player_Sky : public MOVECOM
{
public:
	void Init() override;
	void Update() override;
public:
	//직접
	void Intro();
	void Idle_Up();
	void Idle_Down();
	void Idle_Straight();

	void Trans_Up();
	void Trans_Down();

	void Parry();

	void Trans_Mn();
	void Trans_Lg();

	void Mn_Idle_Up();
	void Mn_Idle_Down();
	void Mn_Idle_Straight();

	void Mn_Trans_Up();
	void Mn_Trans_Down();

	void Ghost();

	//간접
	bool Move();
	void LRAngle();
	void Debug();
	void ParryCheck();
	void ShiftCheck();
	void Shoot();
	void ShootChange();
	float ShootTime = 0.f;
	//fot MonkeyCard
	void MonkeyCard_Back_Check();
public:
	void MonsterEnter(Game_Collision* _This, Game_Collision* _Other);

	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Parry_HitCheck(Game_Collision* _This, Game_Collision* _Other);
	void Parry_Check(Game_Collision* _This, Game_Collision* _Other);


	//편의성 함수
public:
	void Renderer_Setting();
	void Change_Ani(const wchar_t* _AniType);
	bool Ani_Done();
private:
	Player_Sky_State m_eState;
	Player_Sky_ShootType m_ShootType;

	Game_Ptr<Game_Collision> m_Collision_Player_Parry;
	Game_Ptr<Game_Collision> m_Collision_Player_Parry_ForMonster;

	Game_Ptr<Game_Actor> m_Collision_Player_Parry_Debug;
	Game_Ptr<Game_Sprite_Renderer> m_Collision_Player_Parry_Debug_Render;
	Game_Ptr<Game_Sprite_Renderer> m_Collision_Player_Parry_ForMonster_Debug_Render;
	Game_Ptr<Game_Sprite_Renderer>	m_ShootChange_UI_Animation;

	bool m_Parry = false;
	bool m_ShootChange = false;
	float m_HitTime = 10.f;

	float BulletZ = -3.f;

	//For_Monkey_card
	Game_Collision* Other1 = nullptr;
	Game_Collision* Other2 = nullptr;
	bool Card_Back_Check = false;
	float Card_Time = 0.f;
};

