#pragma once
#include "HGAMEPLAYER.h"]
#include "Monkey_Card.h"
#include <vector>
using namespace std;

enum class Monkey_State
{
	Intro,
	Idle_Closed,
	Idle_Open,
	Atk,
	Closed_to_Open,
	Open_to_Closed,
	Shake,
	Death,
	End
};

enum class Monky_Course
{
	Left,
	Right,
	Up,
	Down
};

class Monkey : public  MOVECOM
{
public:
	void Init() override;
	void Update() override;
	void Hit(Game_Collision* _This, Game_Collision* _Other);
	void Renderer_Setting();

public:
	void Move();

	void Intro();
	void Idle_Closed();
	void Idle_Open();
	void Closed_to_Open();
	void Open_to_Closed();
	void Atk();
	void Shake();
	void Death();

	void CardUp();
	void CardDown();

private:
	Monkey_State m_eState = Monkey_State::End;
	Monky_Course m_LR = Monky_Course::Left;
	Monky_Course m_UD = Monky_Course::Up;
	vector<Monkey_Card*> Deck;

	Game_Ptr<Game_Sprite_Renderer>	m_Animation_Claw;
	Game_Ptr<Game_Animation>		m_Animation_Type_Claw;

	vector<Monkey_CardType> Completed_Type;
	vector<Monkey_CardType> Now_Type;

	bool Atk_Ani_Check = false;
	bool CardUpDown_Check = false;
	float TimeCount = 0.f; // 다양하게.
	float OpenCount = 0.f; //Open상태로 있는 시간 누적.
	float Shake_Time = 0.f;

public:
	static bool State_Check;
	static bool Shake_Check;
};

