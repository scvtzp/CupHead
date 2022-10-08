#pragma once
#include "Game_Device.h"
#include <Game_Mgr.h>

class Game_Rasterizer : public Game_Mgr<Game_Rasterizer>
{
public:
	static void Create(const Game_String& _Name, const D3D11_RASTERIZER_DESC& Desc)
	{
		CreateToInsert(_Name, Desc);
	}

private:
	D3D11_RASTERIZER_DESC m_Desc;
	ID3D11RasterizerState* m_State;

public:
	void Create();
	void Setting();

public:
	Game_Rasterizer(const D3D11_RASTERIZER_DESC& _Desc);
	~Game_Rasterizer();

};

