#pragma once
#include "HGAMEPLAYER.h"

class Spread : public MOVECOM
{
public:
	void Init() override;
	void Update() override;

	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Move();
	void Debug();
	void Bullet_Death();

private:
	bool DeathCheck = false;
	float Spread_LifeTime = 0.f;
};

