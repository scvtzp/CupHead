#pragma once
#include "Game_Device.h"

// �ε��� ���۵� ���۷� ġ��
// ���ؽ� ���۵� ���۷� ����
// ��� ���۵� ���۷� ����.

class Game_Device_Buffer
{
protected:
	// D3D11_MAPPED_SUBRESOURCE m_SubRes;
	D3D11_BUFFER_DESC m_BufferDesc;
	ID3D11Buffer* m_pBuffer;

public:
	Game_Device_Buffer() : m_pBuffer(nullptr), m_BufferDesc({0,})
	{
		
	}
	~Game_Device_Buffer() 
	{
		if (nullptr != m_pBuffer)
		{
			m_pBuffer->Release();
		}
	}


};

