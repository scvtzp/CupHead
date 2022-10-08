#pragma once
#pragma once
#include "Game_Device.h"
#include <Game_Mgr.h>

class Game_blend : public Game_Mgr<Game_blend>
{
public:
	static void Create(const Game_String& _Name, const D3D11_BLEND_DESC& Desc)
	{
		CreateToInsert(_Name, Desc);
	}

private:
	D3D11_BLEND_DESC m_Desc;
	ID3D11BlendState* m_State;

public:
	void Create();
	void Setting();

public:
	Game_blend(const D3D11_BLEND_DESC& _Desc);
	~Game_blend();
};

