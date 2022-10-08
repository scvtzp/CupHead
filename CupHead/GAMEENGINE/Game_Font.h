#pragma once
#include <Game_Mgr.h>
#include "Game_Device_Buffer.h"
#include <Game_Ptr.h>
#include <FW1FontWrapper.h>

#pragma comment(lib, "FW1FontWrapper.lib")

class Game_Font : public Game_Mgr<Game_Font>
{
public:
	static void Create(const Game_String& _Name)
	{
		Game_Ptr<Game_Font> Res = CreateToInsert(_Name);
		Res->CreateGameFont(_Name);
	}

private:
	class Game_Font_Start
	{
	public:
		~Game_Font_Start();
	};
	friend Game_Font_Start;
	static Game_Font_Start Starter;

public:
	static void Init();

private:
	static IFW1Factory* m_pFontFactory;
	IFW1FontWrapper* m_pFontWrapper;

public:
	// ����ũ�⸦ Ư���ϰ� �Ž��� ����� ���� �빮��.
	// ������Ʈ�� ���̴��� ����. 
	// �⺻������ ������ ��ǥ�踦 ������� �ϴ� �Լ��� �����̴�. 
	void DrawFont(const Game_String& _Text, float _Size, Game_Vector _Pos, Game_Vector _Color, FW1_TEXT_FLAG _Flag = FW1_TOP);



public:
	Game_Font();
	~Game_Font();

public:
	void CreateGameFont(const Game_String& _Name);
};

