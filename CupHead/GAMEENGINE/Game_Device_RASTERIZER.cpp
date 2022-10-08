#include "Game_Device.h"
#include <Game_Io.h>

#include "Game_Vertex_Shader.h"
#include "Game_Pixel_Shader.h"
#include "DEFVTXHEADER.h"

#include "Game_Device.h"
#include "Game_Rasterizer.h"

#pragma warning( push )
#pragma warning( disable : 26444 )

void Game_Device::RASTERIZERINIT()
{

	{
		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_MODE::D3D11_FILL_SOLID , D3D11_CULL_MODE::D3D11_CULL_BACK , 0};
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;

		Game_Rasterizer::Create(L"BACK", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc =
		{ D3D11_FILL_MODE::D3D11_FILL_WIREFRAME , D3D11_CULL_MODE::D3D11_CULL_BACK , 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;
		Game_Rasterizer::Create(L"WBACK", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_MODE::D3D11_FILL_SOLID , D3D11_CULL_MODE::D3D11_CULL_FRONT , 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;

		Game_Rasterizer::Create(L"FRONT", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc =
		{ D3D11_FILL_MODE::D3D11_FILL_WIREFRAME , D3D11_CULL_MODE::D3D11_CULL_FRONT , 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;
		Game_Rasterizer::Create(L"WFRONT", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = { D3D11_FILL_MODE::D3D11_FILL_SOLID , D3D11_CULL_MODE::D3D11_CULL_NONE , 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;

		Game_Rasterizer::Create(L"NONE", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc =
		{ D3D11_FILL_MODE::D3D11_FILL_WIREFRAME , D3D11_CULL_MODE::D3D11_CULL_NONE , 0 };
		Desc.MultisampleEnable = true;
		Desc.AntialiasedLineEnable = true;
		Game_Rasterizer::Create(L"WNONE", Desc);
	}

}

#pragma warning( pop )