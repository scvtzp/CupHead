#include "Game_VB_Buffer.h"

Game_VB_Buffer::Game_VB_Buffer() : m_Offset(0)
{

}

void Game_VB_Buffer::Create(size_t _Count, size_t _Size, const void* _Data, D3D11_USAGE _USAGE) 
{
	m_Size = (unsigned int)_Size;
	m_Count = (unsigned int)_Count;

	//                       4         32
	m_BufferDesc.ByteWidth = (unsigned int)(_Count * _Size);
	m_BufferDesc.Usage = _USAGE; // ��� ������ �޶�. ���� =�׷���ī�忡 ����

	// �̳༮�� cpu���� �����ҰŴ� ���ҰŴ�.
	// D3D11_USAGE::D3D11_USAGE_DEFAULT �׷��� ī�忡 ����Ǵµ� ���� ���Ѵ�.
	// D3D11_USAGE::D3D11_USAGE_DYNAMIC �׷��� ī�忡 ����Ǵµ� ���� �Ѵ�.
	// 
	if (m_BufferDesc.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DS = {};
	DS.pSysMem = _Data;

	if (S_OK != Game_Device::MAINOBJ()->Device()->CreateBuffer(&m_BufferDesc, &DS, &m_pBuffer))
	{
		AMSG(L"���ؽ� ���� ������ �����߽��ϴ�");
	}
}

void Game_VB_Buffer::Setting() 
{
	// � �ڷ����� �ּҰ��� �����Ҽ� ����
	// � �ڷ����� �ּҰ��� �����Ҽ� ���� �������� �ּҰ��� ������ ���� ������ �ּҰ�.
	// int* const* const* const Ptr = nullptr;

	// ������ 16�� �ε�.
	// 0, 1 1���� �����ϰڴ�.
	// m_pBuffer �� ���ؽ� ���۸�
	// �� ���ؽ� ������ ���ؽ� 1���� ũ��� m_Size
	// �� ���ؽ��� ������ 0������ �����ϰڴ�.
	// 100
	// 50��°���͵� ���� ����.
	Game_Device::MAINOBJ()->Context()->IASetVertexBuffers(0, 1, &m_pBuffer, &m_Size, &m_Offset);
}