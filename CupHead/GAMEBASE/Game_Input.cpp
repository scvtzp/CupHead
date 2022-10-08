#include "Game_Input.h"
#include <Windows.h>
#include "GAMEWINDOW.h"

std::map<Game_String, Game_Input::Game_Key*> Game_Input::AllKey;

Game_Vector Game_Input::PrevPos = Game_Vector::ZERO;
Game_Vector Game_Input::MouseDir = Game_Vector::ZERO;

Game_Input::Game_Static::Game_Static() 
{

}

Game_Input::Game_Static::~Game_Static() 
{
	for (auto& Item: AllKey)
	{
		delete Item.second;
	}
}

// 배웠던 문법의 응용이다.
Game_Input::Game_Static Game_Input::StaticInst;


bool Game_Input::Game_Key::KeyCheck() 
{
	for (size_t i = 0; i < m_KeyVec.size(); i++)
	{
		// 'A' 'B' 
		if (0 == GetAsyncKeyState(m_KeyVec[i]))
		{
			// 하나라도 안눌렸다
			return false;
		}
	}

	// 전부다 눌렸다.
	return true;
}

// 여기서 키 체크를 한다.
void Game_Input::Game_Key::Update()
{
	// 그럼 여기로 오겠죠?
	// 키가 다 눌렸는데?
	if (KeyCheck())
	{
		// 처음 눌렸다.
		if (false == Press)
		{
			Down = true;
			Press = true;
			Up = false;
			Free = false;
		}
		else 
		{
			Down = false;
			Press = true;
			Up = false;
			Free = false;
		// 이전프레임에 눌렸다 계속 눌리고 있었다.
		}
	}
	else 
	{
		// 이전까지 눌리고 있었다.
		if (true == Press)
		{
			Down = false;
			Press = false;
			Up = true;
			Free = true;
		}
		else
		{
			Down = false;
			Press = false;
			Up = false;
			Free = true;
			// 이전프레임에 눌렸다 계속 눌리고 있었다.
		}
	}


}


void Game_Input::Update() 
{
	
	Game_Vector Pos = MousePos3D();
	// 현재위치 빼기 이전위치
	MouseDir = Pos - PrevPos;
	PrevPos = Pos;

	if (MouseDir != Game_Vector::ZERO)
	{
		int a = 0;
	}

	for (auto& Item : AllKey)
	{
		Item.second->Update();
	}
}

Game_Vector Game_Input::MousePos3D() 
{
	return GAMEWINDOW::MAINOBJ()->MousePosTo3D();
}

// 방향과 크기를 동시에 알고 싶을때.
Game_Vector Game_Input::MouseDir3D() {
	return MouseDir;
}

Game_Vector Game_Input::MouseDir3DNormal() 
{
	return MouseDir.NORMAL3DTORETURN();
}