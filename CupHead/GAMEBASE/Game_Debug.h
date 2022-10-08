#pragma once
#include <crtdbg.h>
#include <assert.h>
#include "Game_String.h"
#include <iostream>

#define AMSG(MSSAGE) _wassert(MSSAGE, _CRT_WIDE(__FILE__), (unsigned)(__LINE__))

class GAMEWINDOW;
class Game_Debug
{
	friend GAMEWINDOW;

private:
	// �̳༮�� �������� ������.
	class Game_DebugSTART
	{
	public:
		void StartTest() {}

	public:
		Game_DebugSTART() 
		{
			// �ڵ����� ������ �� üũ�Ѵ�.
			// �ѹ��� ����ǵ��� �ߴ�.
			if (false == IsLeckCheck)
			{
				_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
				IsLeckCheck = true;
			}
		}
		~Game_DebugSTART() 
		{
			if (true == bCon)
			{
				FreeConsole();
			}

			if (nullptr != stream)
			{
				fclose(stream);
			}
		}
	};

	//static void AssertMsg(const Game_String& _Msg) 
	//{
	//	_wassert(_Msg, _crt_wide()
	//}

	// ����� ������ �����̴�.
	static Game_DebugSTART Starter;
	static bool IsLeckCheck;
	// �Լ��� ����ϸ� �� Ŭ������ �ʿ������ϱ�.
	static void LeckCheck() 
	{
		Starter.StartTest();
	}


	static int m_FrameCount;
public:
	static bool bCon;
	static FILE* stream;
	// static HPTR<HGAMEFONT> m_DebugFont;

	static int FrameCount()
	{
		return m_FrameCount;
	}
	static void DebugFrameUpdate()
	{
		++m_FrameCount;
	}

public:

	static void OpenConsole()
	{
		if (0 == AllocConsole())
		{
			AMSG(L"�ܼ�â�� ���µ� �����߽��ϴ�.");
		}
		else
		{
			freopen_s(&stream, "CONIN$", "rb", stdin);
			freopen_s(&stream, "CONOUT$", "wb", stdout);
			freopen_s(&stream, "CONOUT$", "wb", stderr);
			// GetLastError

			// wcout�� �ѱ��̶�� ���� �˷��ش�.
			std::wcout.imbue(std::locale("korean"));
			std::wcout << L"�� ��µǴ��� ���ڽ��ϴ�." << std::endl;
			bCon = true;
		}
	}

	template<typename ... REST>
	static void ConsolePrintText(const Game_String& _Text, REST ... _Arg)
	{
		if (false == bCon)
		{
			AMSG(L"�ܼ�â�� ������ �ʾҽ��ϴ�.");
		}

		wchar_t ArrText[256];
		swprintf_s(ArrText, _Text, _Arg...);
		std::wcout << ArrText << std::endl;
	}
};