#include "Game_Device.h"
#include <Game_Io.h>

#include "Game_Vertex_Shader.h"
#include "Game_Pixel_Shader.h"
#include "DEFVTXHEADER.h"

#include "Game_Device.h"
#include "Game_blend.h"

#pragma warning( push )
#pragma warning( disable : 26444 )

void Game_Device::BLENDINIT()
{
	{
		D3D11_BLEND_DESC Desc = { 0, };

		// ���İ��� ����ؼ� ���� ���۸� ���
		Desc.AlphaToCoverageEnable = FALSE;
		Desc.IndependentBlendEnable = FALSE; // 0���� �ִ� �������θ� ���� �ض�.
		// SV_Target
		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		// Setting �Ҷ� ���� �־��� ���ͷ� ó���ϰ� �Ҽ� �ִ�.
		// D3D11_BLEND::D3D11_BLEND_BLEND_FACTOR
		// D3D11_BLEND::D3D11_BLEND_INV_BLEND_FACTOR

		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

		// 1, 0, 0, 1 * 1, 1, 1, 1
		// 1, 0, 0, 1 + 0, 0, 0, 0

		//Desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		//Desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;

		// ���� 0.5  0
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;

		Game_blend::Create(L"AlphaBlend", Desc);
	}
}

#pragma warning( pop )