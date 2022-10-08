#pragma once
#include "HGAMEPLAYER.h"
class Sky_Bomb : public MOVECOM
{
public:
	Sky_Bomb() {}
	~Sky_Bomb() {}

public:
	void Init() override;
	void Update() override;

	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Move();
	void Debug();

public:

private:
	float m_SpeedY;
};

