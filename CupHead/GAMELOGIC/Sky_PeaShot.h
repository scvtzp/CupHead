#pragma once

#include "HGAMEPLAYER.h"
class Sky_PeaShot : public MOVECOM
{
public:
	void Init() override;
	void Update() override;

	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Move();
	void Debug();

public:
	static int Count;
private:
};
