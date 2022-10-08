#pragma once
#include "HGAMEPLAYER.h"
class Monkey_Bullet  : public MOVECOM
{
public:
	void Init() override;
	void Update() override;

	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Move();
	void Debug();

};

