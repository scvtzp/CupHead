#include "Game_Sampler.h"


Game_Sampler::Game_Sampler(const D3D11_SAMPLER_DESC& _Desc) : m_Desc(_Desc)
{
	Create();
}

Game_Sampler::~Game_Sampler()
{
	if (nullptr != m_State)
	{
		m_State->Release();
	}
}


void Game_Sampler::Create()
{
	if (S_OK != Game_Device::MAINOBJ()->Device()->CreateSamplerState(&m_Desc, &m_State))
	{
		// L"�����Ͷ����� ������Ʈ ������ �����߽��ϴ�"
		// const wchar_t* PTR;
		// PTR + 1;
		AMSG(L"�����Ͷ����� ������Ʈ ������ �����߽��ϴ�.");
	}
}

void Game_Sampler::Setting(SHADERTYPE _Type, unsigned int _Reg)
{
	switch (_Type)
	{
	case SHADERTYPE::SHADER_VS:
		Game_Device::MAINOBJ()->Context()->VSSetSamplers(_Reg, 1, &m_State);
		break;
	case SHADERTYPE::SHADER_HS:
		Game_Device::MAINOBJ()->Context()->HSSetSamplers(_Reg, 1, &m_State);
		break;
	case SHADERTYPE::SHADER_DS:
		Game_Device::MAINOBJ()->Context()->DSSetSamplers(_Reg, 1, &m_State);
		break;
	case SHADERTYPE::SHADER_GS:
		Game_Device::MAINOBJ()->Context()->GSSetSamplers(_Reg, 1, &m_State);
		break;
	case SHADERTYPE::SHADER_PS:
		Game_Device::MAINOBJ()->Context()->PSSetSamplers(_Reg, 1, &m_State);
		break;
	case SHADERTYPE::SHADER_END:
		break;
	default:
		break;
	}
}