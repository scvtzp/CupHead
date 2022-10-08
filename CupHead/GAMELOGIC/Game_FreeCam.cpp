#include "Game_FreeCam.h"


void Game_FreeCam::Init() 
{
	m_Cam = ACTOR()->FindComType<Game_Cam>();

	if (nullptr == m_Cam)
	{
		AMSG(L"����ķ ������Ʈ�� ���Ϳ� ķ�� �������� ��� �����մϴ�.");
	}

	if (nullptr == Game_Input::FindKey(L"CAMLEFT"))
	{
		Game_Input::CreateKey(L"CAMLEFT", 'A');
		Game_Input::CreateKey(L"CAMRIGHT", 'D');
		Game_Input::CreateKey(L"CAMUP", 'Q');
		Game_Input::CreateKey(L"CAMDOWN", 'E');
		Game_Input::CreateKey(L"CAMFOWARD", 'W');
		Game_Input::CreateKey(L"CAMBACK", 'S');

		Game_Input::CreateKey(L"CAMROT", VK_RBUTTON); // ���콺 ������ ��ư
		Game_Input::CreateKey(L"CAMBOOST", VK_LSHIFT); // ���� ��ư
	}

	m_Speed = 10.0F;
	m_RotSpeed = 180.0f;
}

void Game_FreeCam::Update() 
{
	// ������ ��ư ������ �־�߸� �����Դϴ�.
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


	//Game_Input::Press(L"CAMROT", VK_RBUTTON); // ���콺 ������ ��ư
	//Game_Input::Press(L"CAMROT", VK_LSHIFT); // ���� ��ư
}