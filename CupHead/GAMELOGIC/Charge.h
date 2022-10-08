#pragma once
#include "HGAMEPLAYER.h"

class Charge : public MOVECOM
{
public:
	void Init() override;
	void Update() override;

	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Move();
	void Debug();
	void Bullet_Death();
	void Set_Type(bool B) { ChargeType = B; }

private:
	bool DeathCheck = false;
	float Spread_LifeTime = 0.f;

	bool ChargeType = false; //true-lg /false-sm
};