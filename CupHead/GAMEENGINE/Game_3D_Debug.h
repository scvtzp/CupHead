#pragma once
#include <Game_String.h>
#include "Game_Font.h"
#include <vector>
#include <Game_Debug.h>

class Game_3D_Debug
{
private:
	static Game_Ptr<Game_Font> m_DebugFont;
	static float m_BasicFontSize;
	static Game_Vector m_BasicStartPos;
	static Game_Vector m_CurTextSize;
	static Game_Vector m_BasicColor;
public:
	static void Init(bool _IsOpenConsole = false);

private:
	class DebugTextInfo
	{
	public:
		Game_String Text;
		float Size;
		Game_Vector Pos;
		Game_Vector Color;
	};

	static std::vector<DebugTextInfo> m_AllDebugText;



public:

	static void DrawDebugText(const Game_String& _Text, float _Size, Game_Vector _Pos, Game_Vector _Color);

	template<typename ... REST>
	static void DrawDebugText(const Game_String& _Text, REST ... _Arg)
	{
		wchar_t ArrText[256];
		swprintf_s(ArrText, _Text, _Arg...);
		Game_3D_Debug::DrawDebugText(ArrText, m_BasicFontSize, m_CurTextSize, m_BasicColor);
		m_CurTextSize.Y += m_BasicFontSize;
	}

	template<typename ... REST>
	static void ConsolePrintText(const Game_String& _Text, REST ... _Arg)
	{
		Game_Debug::ConsolePrintText(_Text, _Arg...);
	}

public:

	static void OpenConsole()
	{
		Game_Debug::OpenConsole();
	}

public:
	static void DebugRender();

};