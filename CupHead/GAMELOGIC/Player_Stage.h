#pragma once
#include "HGAMEPLAYER.h"
enum class Player_Stage_State
{
	Intro,
	Idle,
	Walk,
	Jump,
	Down,
	Dash,
	End
};

enum class Player_Inventory
{
	Peashot,
	Charge,
	Spread,

	End
};

class Player_Stage :
	public MOVECOM
{
private:
	Player_Stage_State m_eState;
	Player_Inventory m_Inven[4] = {Player_Inventory::Peashot, Player_Inventory::Spread, Player_Inventory::Peashot, Player_Inventory::Peashot};
	Player_Inventory m_ShootType = Player_Inventory::End;

	Game_Ptr<Game_Collision> m_Collision_Player_Parry;
	Game_Ptr<Game_Collision> m_Collision_Player_Parry_ForMonster;

	Game_Ptr<Game_Actor> m_Collision_Player_Parry_Debug;
	Game_Ptr<Game_Sprite_Renderer> m_Collision_Player_Parry_Debug_Render;
	Game_Ptr<Game_Sprite_Renderer> m_Collision_Player_Parry_ForMonster_Debug_Render;

	bool m_JumpCheck1 = false;
	bool m_JumpCheck2 = false;
	bool m_Parry = false;
	bool Parry_Count_Check = false;
	bool m_Hit = false;
	float m_HitTime = 0.f;
	bool DownCheck = false;
	bool DownShotCheck = false;
	float DashTime = 0.f;
	bool DashStartAni_Check = false;

	//BULLET
	float Weapon_Z = -1.f;
	float Atk_CoolTime = 0.f;
	float Charge_Time = 0.f;
	Game_Ptr<Game_Sprite_Renderer>	Charge_Ani;
	Game_Ptr<Game_Animation>		Charge_Ani_Type;

public:
	void Init() override;
	void Update() override;
public:
	void Intro();

	void Idle();
	void Move();
	void Jump();
	void JumpCheck();
	void Shoot();
	void ShootChange();
	void Down();
	void Dash();
	void Debug();
	void Gravity();
	void YCheck();

public:
	void MonsterEnter(Game_Collision* _This, Game_Collision* _Other);
	
	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Parry_HitCheck(Game_Collision* _This, Game_Collision* _Other);
	void Parry_Check(Game_Collision* _This, Game_Collision* _Other);


	//편의성 함수
public:
	void Change_Ani(const wchar_t* _AniType);
};

