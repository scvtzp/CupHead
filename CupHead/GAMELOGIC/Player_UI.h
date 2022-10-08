#pragma once
#include "HGAMEPLAYER.h"

enum class Bullet_Type
{
	Peashot,
	Bouncer,
	Charge,
	Spread,
	Sky_Bomb,
	Sky_Peashot,

	End
};

class Player_UI : public MOVECOM
{
private:

public:
	void Init() override;
	void Update() override;

	static int Hp;
	static void Set_Hp(int a) { Hp = a; }

	static Bullet_Type BulletType;
	Bullet_Type Past_BulletType;
	static void Set_Bullet(Bullet_Type BT) { BulletType = BT; }
	
	float Bullet_PopUp_Timer = 0.f;
	float HpTime = 0.f;

	bool Hp1Check = false;
};

