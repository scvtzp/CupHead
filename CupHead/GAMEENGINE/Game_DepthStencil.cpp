#include "Game_DepthStencil.h"

Game_DepthStencil::Game_DepthStencil(const D3D11_DEPTH_STENCIL_DESC& _Desc) : m_Desc(_Desc)
{
	Create();
}

Game_DepthStencil::~Game_DepthStencil() 
{
	if (nullptr != m_State)
	{
		m_State->Release();
	}
}


void Game_DepthStencil::Create()
{
	if (S_OK != Game_Device::MAINOBJ()->Device()->CreateDepthStencilState(&m_Desc, &m_State))
	{
		// L"�����Ͷ����� ������Ʈ ������ �����߽��ϴ�"
		// const wchar_t* PTR;
		// PTR + 1;
		AMSG(L"�����Ͷ����� ������Ʈ ������ �����߽��ϴ�.");
	}
}

void Game_DepthStencil::Setting()
{
	//;


	// ���̷�Ʈ 11�� ������ 4���� ���Դ�. 
	Game_Device::MAINOBJ()->Context()->OMSetDepthStencilState(m_State, 0);
}