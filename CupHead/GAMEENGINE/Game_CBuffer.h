#pragma once
#include <vector>
#include <Game_Mgr.h>
#include "Game_Device_Buffer.h"
#include <Game_Ptr.h>
#include "Game_Shader.h"

class Game_CBuffer : public Game_Mgr<Game_CBuffer>, public Game_Device_Buffer
{
public:
	template<typename T>
	static void Create(const Game_String& _Name/*, D3D11_USAGE _USAGE = D3D11_USAGE::D3D11_USAGE_DYNAMIC*/)
	{
		Game_Ptr<Game_CBuffer> Res = CreateToInsert(_Name);
		Res->Create(sizeof(T));
	}

private:
	D3D11_MAPPED_SUBRESOURCE m_SubData;

public:
	void Create(unsigned int _Size);

	void Setting(SHADERTYPE _Type, unsigned int _Reg);

	template<typename T>
	void DataChange(const T& _Data)
	{
		DataChange((void*)&_Data, sizeof(T));
	}

	void DataChange(void* _Data, unsigned int _Size);
};