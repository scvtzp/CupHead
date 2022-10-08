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
	// 이녀석을 쓰는일이 없었다.
	class Game_DebugSTART
	{
	public:
		void StartTest() {}

	public:
		Game_DebugSTART() 
		{
			// 자동으로 무조건 릭 체크한다.
			// 한번만 실행되도록 했다.
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

	// 배웠던 문법의 응용이다.
	static Game_DebugSTART Starter;
	static bool IsLeckCheck;
	// 함수를 사용하면 이 클래스가 필요해지니까.
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
			AMSG(L"콘솔창을 여는데 실패했습니다.");
		}
		else
		{
			freopen_s(&stream, "CONIN$", "rb", stdin);
			freopen_s(&stream, "CONOUT$", "wb", stdout);
			freopen_s(&stream, "CONOUT$", "wb", stderr);
			// GetLastError

			// wcout에 한국이라고 직접 알려준다.
			std::wcout.imbue(std::locale("korean"));
			std::wcout << L"잘 출력되는지 보겠습니다." << std::endl;
			bCon = true;
		}
	}

	template<typename ... REST>
	static void ConsolePrintText(const Game_String& _Text, REST ... _Arg)
	{
		if (false == bCon)
		{
			AMSG(L"콘솔창이 열리지 않았습니다.");
		}

		wchar_t ArrText[256];
		swprintf_s(ArrText, _Text, _Arg...);
		std::wcout << ArrText << std::endl;
	}
};