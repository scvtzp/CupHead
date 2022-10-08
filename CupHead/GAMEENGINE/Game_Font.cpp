#include "Game_Font.h"
#include "Game_Device.h"

IFW1Factory* Game_Font::m_pFontFactory = nullptr;

Game_Font::Game_Font_Start Game_Font::Starter;

void Game_Font::Init()
{
	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_pFontFactory))
	{
		AMSG(L"폰트 팩토리 생성에 실패했습니다.");
	}
}

Game_Font::Game_Font_Start::~Game_Font_Start()
{
	if (nullptr != m_pFontFactory)
	{
		m_pFontFactory->Release();
	}
}



Game_Font::Game_Font() : m_pFontWrapper(nullptr)
{
}
Game_Font::~Game_Font()
{
	if (nullptr != m_pFontWrapper)
	{
		m_pFontWrapper->Release();
	}
}

void Game_Font::CreateGameFont(const Game_String& _Name)
{
	if (S_OK != m_pFontFactory->CreateFontWrapper(Game_Device::MAINOBJ()->Device(), _Name, &m_pFontWrapper))
	{
		AMSG(L"폰트 생성에 실패했습니다. " + _Name);
	}
}

void Game_Font::DrawFont(const Game_String& _Text, float _Size, Game_Vector _Pos, Game_Vector _Color, FW1_TEXT_FLAG _Flag)
{
	m_pFontWrapper->DrawString(Game_Device::MAINOBJ()->Context(), _Text, _Size, _Pos.X, _Pos.Y, _Color.ColorToUint(), _Flag);
}