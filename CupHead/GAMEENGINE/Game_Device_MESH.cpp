#include "Game_Device.h"
#include "Game_VB_Buffer.h"
#include "Game_IB_Buffer.h"
#include "DEFVTXHEADER.h"

void Game_Device::MESHINIT() 
{
	{
		std::vector<VTX2DIMGIN> m_Color;
		m_Color.resize(4);
		// �⺻ �Ž��� ���鶧�� ũ�⸦ 1�� ����� ���� ��찡 ��κ��̴�.
		m_Color[0].Pos = { -0.5f, 0.5f, 0.0f, 1.0f };
		m_Color[1].Pos = { 0.5f, 0.5f, 0.0f, 1.0f };
		m_Color[2].Pos = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Color[3].Pos = { -0.5f, -0.5f, 0.0f, 1.0f };

		m_Color[0].Uv = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_Color[1].Uv = { 1.0f, 0.0f, 0.0f, 1.0f };
		m_Color[2].Uv = { 1.0f, 1.0f, 0.0f, 1.0f };
		m_Color[3].Uv = { 0.0f, 1.0f, 0.0f, 1.0f };

		m_Color[0].Color = m_Color[1].Color = m_Color[2].Color = m_Color[3].Color = Game_Vector::WHITE;
		Game_VB_Buffer::Create<VTX2DIMGIN>(L"2DCOLORRECT", m_Color);

		// �ð�������� �׸��ٴ°� �߿��ϴ�.
		// �޸��� �ȱ׸��� �ϴ°� �� �׷����� ���⿡ ���ؼ� �����ȴ�.
		std::vector<UINT> m_IDX = {0, 1, 2, 0, 2, 3};
		Game_IB_Buffer::Create<UINT>(L"2DCOLORRECT", m_IDX);
	}
}