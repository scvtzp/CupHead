#pragma once
#include "Monkey.h"
#include "HGAMEPLAYER.h"

enum class Monkey_CardType
{
	Cuphead,
	MugMan,
	Chips,
	Flowers,
	Shield,
	Spindle,
	End
};

class Monkey_Card : public MOVECOM
{
public:
	void Init() override;
	void Update() override;
	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Renderer_Setting();
	void Renderer_Update();
	void Effect(); //띠이용 하는 이펙트.
	//몽키에서 접근용.
	bool Card_Collision_Get_Check() { return m_Collision->Get_Check(); }
	
	//가운데 점 출력
	Game_Ptr<Game_Sprite_Renderer> Dot_Renderer;

	void Set_CardType(Monkey_CardType Type) {	CardType = Type;	}
	Monkey_CardType Get_CardType() { return CardType; }

private:
	Monkey_CardType CardType = Monkey_CardType::End;
	bool ParryCheck = false;
	bool Change_Check = false;
	float Effect_Time = 0.f;
	bool Effect_Check = false;
	bool Effect_On = true;

public:
	static Monkey_CardType Card_1; 
	static Monkey_CardType Card_2;
};

