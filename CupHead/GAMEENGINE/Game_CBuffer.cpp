#include "Game_CBuffer.h"
//
void Game_CBuffer::Setting(SHADERTYPE _Type, unsigned int _Reg) 
{
	switch (_Type)
	{
	case SHADERTYPE::SHADER_VS:
		Game_Device::MAINOBJ()->Context()->VSSetConstantBuffers(_Reg, 1, &m_pBuffer);
		break;
	case SHADERTYPE::SHADER_HS:
		Game_Device::MAINOBJ()->Context()->HSSetConstantBuffers(_Reg, 1, &m_pBuffer);
		break;
	case SHADERTYPE::SHADER_DS:
		Game_Device::MAINOBJ()->Context()->DSSetConstantBuffers(_Reg, 1, &m_pBuffer);
		break;
	case SHADERTYPE::SHADER_GS:
		Game_Device::MAINOBJ()->Context()->GSSetConstantBuffers(_Reg, 1, &m_pBuffer);
		break;
	case SHADERTYPE::SHADER_PS:
		Game_Device::MAINOBJ()->Context()->PSSetConstantBuffers(_Reg, 1, &m_pBuffer);
		break;
	case SHADERTYPE::SHADER_END:
		break;
	default:
		break;
	}
}

void Game_CBuffer::Create(unsigned int _Size)
{
	m_BufferDesc.ByteWidth = _Size;
	m_BufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC; // ��� ������ �޶�. ���� =�׷���ī�忡 ����
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (S_OK != Game_Device::MAINOBJ()->Device()->CreateBuffer(&m_BufferDesc, nullptr, &m_pBuffer))
	{
		AMSG(L"������� ���ÿ� �����߽��ϴ�");
	}
}

void Game_CBuffer::DataChange(void* _Data, unsigned int _Size)
{
	if (m_BufferDesc.ByteWidth != _Size)
	{
		AMSG(NAME() + L" ��������� ũ�Ⱑ �ٸ��ϴ�.");
	}
	// �������� �����̴ϱ� �װ� �ɱ��?
	// �׷���ī��� ���� m_pBuffer������ ������ �����Ұž�.
	// �׷���ī��� �������� ���ؼ�
	// ��Ƽ ������ ȯ���� ��츦 ����ؼ�
	// �̳༮�� ������ ������� ���ϰ� ���´�.
	// ���������
	Game_Device::MAINOBJ()->Context()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_SubData);
	memcpy_s(m_SubData.pData, m_BufferDesc.ByteWidth, _Data, m_BufferDesc.ByteWidth);
	// ������ ��ȯ���ش�.
	Game_Device::MAINOBJ()->Context()->Unmap(m_pBuffer, 0);

}