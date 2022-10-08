#include "Game_Device.h"
#include "Game_VB_Buffer.h"
#include "Game_IB_Buffer.h"
#include "DEFVTXHEADER.h"

void Game_Device::MESHINIT() 
{
	{
		std::vector<VTX2DIMGIN> m_Color;
		m_Color.resize(4);
		// 기본 매쉬를 만들때는 크기를 1로 만들어 놓는 경우가 대부분이다.
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

		// 시계방향으로 그린다는게 중요하다.
		// 뒷면은 안그리게 하는게 이 그려지는 방향에 의해서 결정된다.
		std::vector<UINT> m_IDX = {0, 1, 2, 0, 2, 3};
		Game_IB_Buffer::Create<UINT>(L"2DCOLORRECT", m_IDX);
	}
}