#include "Game_3D_Debug.h"

std::vector<Game_3D_Debug::DebugTextInfo> Game_3D_Debug::m_AllDebugText;
Game_Ptr<Game_Font> Game_3D_Debug::m_DebugFont;

float Game_3D_Debug::m_BasicFontSize = 20.0f;
Game_Vector Game_3D_Debug::m_BasicStartPos = Game_Vector(0.0F, 0.0F, 0.0F, 0.0F);
Game_Vector Game_3D_Debug::m_CurTextSize = Game_Vector(0.0F, 0.0F, 0.0F, 0.0F);
Game_Vector Game_3D_Debug::m_BasicColor = Game_Vector::RED;

void Game_3D_Debug::Init(bool _IsOpenConsole)
{
	Game_Font::Init();

	m_DebugFont = new Game_Font();
	m_DebugFont->CreateGameFont(L"±¼¸²");

	m_AllDebugText.reserve(100);

	if (true == _IsOpenConsole)
	{
		OpenConsole();
	}
}


void Game_3D_Debug::DrawDebugText(const Game_String& _Text, float _Size, Game_Vector _Pos, Game_Vector _Color)
{
	m_AllDebugText.push_back({ _Text, _Size, _Pos, _Color });
}


void Game_3D_Debug::DebugRender()
{

	for (size_t i = 0; i < m_AllDebugText.size(); i++)
	{
		m_DebugFont->DrawFont(m_AllDebugText[i].Text, m_AllDebugText[i].Size, m_AllDebugText[i].Pos, m_AllDebugText[i].Color);
	}

	m_CurTextSize = m_BasicStartPos;

	m_AllDebugText.clear();

	Game_Device::Reset();
}