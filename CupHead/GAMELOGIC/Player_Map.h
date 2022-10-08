#pragma once
#include "HGAMEPLAYER.h"

enum class Player_Map_Animation_Type
{
	Idle_Down,
	Idle_Up,

	Idle_R,
	Idle_R_Down_Side,
	Idle_R_Up_Side,

	Idle_L,
	Idle_L_Down_Side,
	Idle_L_Up_Side,

	Move_Up,
	Move_Down,

	Move_R,
	Move_R_Up,
	Move_R_Down,

	Move_L,
	Move_L_Up,
	Move_L_Down,

	End
};

class Player_Map : public MOVECOM
{
private:
	Game_Ptr<Game_Cam> m_Cam;

	Game_Vector ColorCheckDir[4];
	bool ArrColorCheck[4];

	Player_Map_Animation_Type Past_AnimationType = Player_Map_Animation_Type::End;
	Player_Map_Animation_Type Present_AnimationType = Player_Map_Animation_Type::End;

public:
	void Init() override;
	void Update() override;
	void Collision();
	void Hit(Game_Collision* _This, Game_Collision* _Other);

	void Renderer_Setting();
	void Debug();

	static bool Player_Entrance_Check;
	static bool Get_Entrance_Check() { return Player_Entrance_Check; }

	static Game_Vector Player_Pos;
	static bool MoveCheck;

	static int Gold;
};

