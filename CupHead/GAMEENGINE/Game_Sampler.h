#pragma once
#include "Game_Shader.h"
#include <Game_Mgr.h>

class Game_Sampler : public Game_Mgr<Game_Sampler>
{
public:
	static void Create(const Game_String& _Name, const D3D11_SAMPLER_DESC& Desc)
	{
		CreateToInsert(_Name, Desc);
	}

private:
	D3D11_SAMPLER_DESC m_Desc;
	ID3D11SamplerState* m_State;

public:
	void Create();
	void Setting(SHADERTYPE _Type, unsigned int _Reg);

public:
	Game_Sampler(const D3D11_SAMPLER_DESC& _Desc);
	~Game_Sampler();
};

