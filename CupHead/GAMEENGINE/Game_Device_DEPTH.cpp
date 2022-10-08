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

		//if (먼저 그린애의z > 지금 그려지는애z)
		//{
		//	그려진다
		//}
		//else {
		//	그려지지 않는다.
		//}
		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		// 체크
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;


		Game_DepthStencil::Create(L"DEFDEPTH", Desc);
	}
}

#pragma warning( pop )