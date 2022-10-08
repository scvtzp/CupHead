#include "Game_Device.h"
#include <Game_Io.h>

#include "Game_Vertex_Shader.h"
#include "Game_Pixel_Shader.h"
#include "DEFVTXHEADER.h"

#include "Game_Device.h"
#include "Game_DepthStencil.h"

#pragma warning( push )
#pragma warning( disable : 26444 )

void Game_Device::DEPTHINIT()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0, };

		//if (���� �׸�����z > ���� �׷����¾�z)
		//{
		//	�׷�����
		//}
		//else {
		//	�׷����� �ʴ´�.
		//}
		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		// üũ
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;


		Game_DepthStencil::Create(L"DEFDEPTH", Desc);
	}
}

#pragma warning( pop )