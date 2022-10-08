#pragma once
#include "Game_Device.h"
#include <Game_Mgr.h>

class Game_DepthStencil : public Game_Mgr<Game_DepthStencil>
{
public:
	static void Create(const Game_String& _Name, const D3D11_DEPTH_STENCIL_DESC& Desc)
	{
		CreateToInsert(_Name, Desc);
	}

private:
	D3D11_DEPTH_STENCIL_DESC m_Desc;
	ID3D11DepthStencilState* m_State;

public:
	void Create();
	void Setting();

public:
	Game_DepthStencil(const D3D11_DEPTH_STENCIL_DESC& _Desc);
	~Game_DepthStencil();
};

