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

// ����� ������ �����̴�.
Game_Input::Game_Static Game_Input::StaticInst;


bool Game_Input::Game_Key::KeyCheck() 
{
	for (size_t i = 0; i < m_KeyVec.size(); i++)
	{
		// 'A' 'B' 
		if (0 == GetAsyncKeyState(m_KeyVec[i]))
		{
			// �ϳ��� �ȴ��ȴ�
			return false;
		}
	}

	// ���δ� ���ȴ�.
	return true;
}

// ���⼭ Ű üũ�� �Ѵ�.
void Game_Input::Game_Key::Update()
{
	// �׷� ����� ������?
	// Ű�� �� ���ȴµ�?
	if (KeyCheck())
	{
		// ó�� ���ȴ�.
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
		// ���������ӿ� ���ȴ� ��� ������ �־���.
		}
	}
	else 
	{
		// �������� ������ �־���.
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
			// ���������ӿ� ���ȴ� ��� ������ �־���.
		}
	}


}


void Game_Input::Update() 
{
	
	Game_Vector Pos = MousePos3D();
	// ������ġ ���� ������ġ
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

// ����� ũ�⸦ ���ÿ� �˰� ������.
Game_Vector Game_Input::MouseDir3D() {
	return MouseDir;
}

Game_Vector Game_Input::MouseDir3DNormal() 
{
	return MouseDir.NORMAL3DTORETURN();
}