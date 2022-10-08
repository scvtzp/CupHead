#include "HGAMEPLAYER.h"

#include "Game_FreeCam.h"


void HGAMEPLAYER::Init()
{
	m_Gravity = 0.5F;
	m_Speed = 1.0F;
	m_Cam = SCENE()->MainCam();

	m_eState = PLAYERSTATE::PS_IDLE;
}

void HGAMEPLAYER::JumpCheck()
{
	if (Game_Input::Down(L"JUMP"))
	{
		GRAVITYVEC = { 0,15.0F,0 };
		JUMPVEC = { 0,10.0F,0 };
		m_eState = PLAYERSTATE::PS_JUMP;
	}
}


void HGAMEPLAYER::KeyMove() {
	if (Game_Input::Press(L"PLAYERLEFT"))
	{
		TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
		m_Cam->TRANS()->WMOVE(Game_Vector::LEFT * Game_Time::DeltaTime(m_Speed));
	}
	if (Game_Input::Press(L"PLAYERRIGHT"))
	{
		TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
		m_Cam->TRANS()->WMOVE(Game_Vector::RIGHT * Game_Time::DeltaTime(m_Speed));
	}
}
void HGAMEPLAYER::Update()
{
	switch (m_eState)
	{
	case PLAYERSTATE::PS_IDLE:
		IDLE();
		break;
	case PLAYERSTATE::PS_WALK:
		WALK();
		break;
	case PLAYERSTATE::PS_JUMP:
		JUMP();
		break;
	default:
		break;
	}
}
void HGAMEPLAYER::IDLE() {
	if (Game_Input::Press(L"PLAYERLEFT") || Game_Input::Press(L"PLAYERRIGHT"))
	{
		m_eState = PLAYERSTATE::PS_WALK;
	}
	JumpCheck();
}
void HGAMEPLAYER::WALK()
{
	KeyMove();
	JumpCheck();
}
void HGAMEPLAYER::JUMP()
{
	KeyMove();
	// 점프가 상태가 되면 점프 상태만 보고 거기만 집중하면 된다.
	TRANS()->WMOVE(JUMPVEC * Game_Time::DeltaTime());
	JUMPVEC -= GRAVITYVEC * Game_Time::DeltaTime();
	// 땅에 닿았다면
	// 뭐가 어쨌다면
}