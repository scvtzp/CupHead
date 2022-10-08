#include "Game_Device.h"
#include <Game_Io.h>

#include "Game_Vertex_Shader.h"
#include "Game_Pixel_Shader.h"
#include "DEFVTXHEADER.h"

#include "Game_Device.h"

#pragma warning( push )
#pragma warning( disable : 26444 )

void Game_Device::SHADERINIT() 
{
	Game_Directory m_Dir;
	m_Dir.MoveParent(L"CupHead");
	m_Dir.Move(L"SHADER");

	{
		Game_Ptr<Game_Vertex_Shader> VTX = Game_Vertex_Shader::Load(m_Dir.PlusFileName(L"2DIMG.hlsl"), L"VS_2DIMG");
		VTX->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
		VTX->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
		VTX->AddLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
		VTX->CreateLayout();
		Game_Pixel_Shader::Load(m_Dir.PlusFileName(L"2DIMG.hlsl"), L"PS_2DIMG");
	}
}

#pragma warning( pop )