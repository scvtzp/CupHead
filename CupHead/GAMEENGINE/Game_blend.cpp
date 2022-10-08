#include "Game_blend.h"

Game_blend::Game_blend(const D3D11_BLEND_DESC& _Desc) : m_Desc(_Desc)
{
	Create();
}

Game_blend::~Game_blend()
{
	if (nullptr != m_State)
	{
		m_State->Release();
	}
}


void Game_blend::Create()
{
	if (S_OK != Game_Device::MAINOBJ()->Device()->CreateBlendState(&m_Desc, &m_State))
	{
		// L"�����Ͷ����� ������Ʈ ������ �����߽��ϴ�"
		// const wchar_t* PTR;
		// PTR + 1;
		AMSG(L"�����Ͷ����� ������Ʈ ������ �����߽��ϴ�.");
	}
}

void Game_blend::Setting()
{
	// ���̷�Ʈ 11�� ������ 4���� ���Դ�. 
	Game_Device::MAINOBJ()->Context()->OMSetBlendState(m_State, nullptr, 0xffffffff);
}