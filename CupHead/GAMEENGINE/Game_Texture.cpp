#include "Game_Texture.h"

Game_Texture::Game_Texture() : m_SRV(nullptr), m_RTV(nullptr), m_DSV(nullptr)
{
}
Game_Texture::~Game_Texture() 
{
	if (nullptr != m_SRV) { m_SRV->Release();};
	if(nullptr != m_RTV	) { m_RTV->Release();};
	if(nullptr != m_DSV	) { m_DSV->Release();};
	if(nullptr != m_pTex) {m_pTex->Release();};

}

void Game_Texture::Load() 
{
	Game_String Ext = GameFile.Ext();

	// Ȯ������ ��ҹ��� ���л������ ó���Ҽ� �ְ� �غ�����????
	// ��� �ϸ� �ɱ�?
	if (Ext == L"dds")
	{
		
	}
	else if(Ext == L"tga")
	{

	}
	else 
	{
		// �ε��� �Ѱ��� ������ ���� ���°� �ƴϴ�.
		if (S_OK != DirectX::LoadFromWICFile(GameFile.FullPath(), DirectX::WIC_FLAGS_NONE, nullptr, m_Img))
		{
			AMSG(GameFile.FullPath() + L" : �ؽ�ó �ε��� �����߽��ϴ�.");
			return;
		}
	}

	// m_SRV
	if (S_OK != DirectX::CreateShaderResourceView	(Game_Device::MAINOBJ()->Device(), m_Img.GetImages(), m_Img.GetImageCount(), m_Img.GetMetadata(), &m_SRV))
	{
		AMSG(GameFile.FullPath() + L" : ���̴� ���ҽ� ������ �����߽��ϴ�.");
		return;
	}

	Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	Desc.Width = (unsigned int)m_Img.GetMetadata().width;
	Desc.Height = (unsigned int)m_Img.GetMetadata().height;

}

Game_Vector Game_Texture::Size() 
{
	return Game_Vector((float)Desc.Width, (float)Desc.Height, 1.0F );
}

void Game_Texture::Setting(SHADERTYPE _Type, unsigned int _Reg)
{
	switch (_Type)
	{
	case SHADERTYPE::SHADER_VS:
		Game_Device::MAINOBJ()->Context()->VSSetShaderResources(_Reg, 1, &m_SRV);
		break;
	case SHADERTYPE::SHADER_HS:
		Game_Device::MAINOBJ()->Context()->HSSetShaderResources(_Reg, 1, &m_SRV);
		break;
	case SHADERTYPE::SHADER_DS:
		Game_Device::MAINOBJ()->Context()->DSSetShaderResources(_Reg, 1, &m_SRV);
		break;
	case SHADERTYPE::SHADER_GS:
		Game_Device::MAINOBJ()->Context()->GSSetShaderResources(_Reg, 1, &m_SRV);
		break;
	case SHADERTYPE::SHADER_PS:
		Game_Device::MAINOBJ()->Context()->PSSetShaderResources(_Reg, 1, &m_SRV);
		break;
	case SHADERTYPE::SHADER_END:
		break;
	default:
		break;
	}
}

Game_Vector Game_Texture::GetPixel(Game_Vector _Pos)
{
	// ĳ���ʹ� �����Ʈ? 1����Ʈ

	if (0 > _Pos.X || 0 > _Pos.Y)
	{
		return Game_Vector::BLACK;
		AMSG(L"- �ȼ��� �������� �ʽ��ϴ�.");
	}

	if (Desc.Width <= _Pos.UIX() || Desc.Height <= _Pos.UIY())
	{
		return Game_Vector::BLACK;
		AMSG(L"�̹��� �ٱ� �ȼ��� ������ ���������� �߽��ϴ�.");
	}

	uint8_t* FirstColor = m_Img.GetPixels();
	// �̹����� ���� ������ �����ü� �ִ�.
	DXGI_FORMAT Fmt = m_Img.GetMetadata().format;
	Game_Vector Color = Game_Vector::BLACK;
	switch (Fmt)
	{
	case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
		FirstColor = &FirstColor[(Desc.Width * _Pos.UIY() + _Pos.UIX()) * 4];
		Color = Game_Vector::ColorToUintInt8(FirstColor[2], FirstColor[1], FirstColor[0], FirstColor[3]);
		break;
	case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		FirstColor = &FirstColor[(Desc.Width * _Pos.UIY() + _Pos.UIX()) * 4];
		Color = Game_Vector::ColorToUintInt8(FirstColor[2], FirstColor[1], FirstColor[0], FirstColor[3]);
		break;
	default:
		AMSG(L"ó���Ҽ� ���� �����Դϴ�.");
		break;
	}

	// 0, 0�ȼ��� r�� �������̴�

	// ���� �⺻���� ������ ����
	// ���ȼ��� �ǳʶٰ� ������ ���� 4�� �ؾ��Ѵ�.
	// FirstColor += 4;
	// 0, 0�ȼ��� g�� ������
	return Color;

}
