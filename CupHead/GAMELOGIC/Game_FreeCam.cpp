#include "Game_FreeCam.h"


void Game_FreeCam::Init() 
{
	m_Cam = ACTOR()->FindComType<Game_Cam>();

	if (nullptr == m_Cam)
	{
		AMSG(L"프리캠 컴포넌트는 액터에 캠이 있을때만 사용 가능합니다.");
	}

	if (nullptr == Game_Input::FindKey(L"CAMLEFT"))
	{
		Game_Input::CreateKey(L"CAMLEFT", 'A');
		Game_Input::CreateKey(L"CAMRIGHT", 'D');
		Game_Input::CreateKey(L"CAMUP", 'Q');
		Game_Input::CreateKey(L"CAMDOWN", 'E');
		Game_Input::CreateKey(L"CAMFOWARD", 'W');
		Game_Input::CreateKey(L"CAMBACK", 'S');

		Game_Input::CreateKey(L"CAMROT", VK_RBUTTON); // 마우스 오른쪽 버튼
		Game_Input::CreateKey(L"CAMBOOST", VK_LSHIFT); // 가속 버튼
	}

	m_Speed = 10.0F;
	m_RotSpeed = 180.0f;
}

void Game_FreeCam::Update() 
{
	// 오른쪽 버튼 누르고 있어야만 움직입니다.
	if (true == Game_Input::Press(L"CAMROT"))
	{
		Game_Vector DIR = Game_Input::MouseDir3D();
		TRANS()->WROTXADDY(DIR.X * Game_Time::DeltaTime(m_RotSpeed));
		TRANS()->WROTXADDX(-DIR.Y * Game_Time::DeltaTime(m_RotSpeed));

	}


	if (Game_Input::Press(L"CAMLEFT"))
	{
		TRANS()->WMOVE(TRANS()->WLEFT() * Game_Time::DeltaTime(m_Speed));
	}
	if (Game_Input::Press(L"CAMRIGHT"))
	{
		TRANS()->WMOVE(TRANS()->WRIGHT() * Game_Time::DeltaTime(m_Speed));
	}
	if (Game_Input::Press(L"CAMUP"))
	{
		TRANS()->WMOVE(TRANS()->WUP() * Game_Time::DeltaTime(m_Speed));
	}
	if (Game_Input::Press(L"CAMDOWN"))
	{
		TRANS()->WMOVE(TRANS()->WDOWN() * Game_Time::DeltaTime(m_Speed));
	}
	if (Game_Input::Press(L"CAMFOWARD"))
	{
		TRANS()->WMOVE(TRANS()->WFOWARD() * Game_Time::DeltaTime(m_Speed));
	}
	if (Game_Input::Press(L"CAMBACK"))
	{
		TRANS()->WMOVE(TRANS()->WBACK() * Game_Time::DeltaTime(m_Speed));
	}


	//Game_Input::Press(L"CAMROT", VK_RBUTTON); // 마우스 오른쪽 버튼
	//Game_Input::Press(L"CAMROT", VK_LSHIFT); // 가속 버튼
}