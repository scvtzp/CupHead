#pragma once
// 점 RECT
// 집합 다수 => 자료구조
#include <vector>
#include <Game_Mgr.h>
#include "Game_Device_Buffer.h"
#include <Game_Ptr.h>
// 포지션



class Game_VB_Buffer : public Game_Mgr<Game_VB_Buffer>, public Game_Device_Buffer
{
public:
	template<typename T>
	static void Create(const Game_String& _Name, std::vector<T> _Vtx, D3D11_USAGE _USAGE = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		Game_Ptr<Game_VB_Buffer> Res = CreateToInsert(_Name);
		Res->Create(_Vtx.size(), sizeof(T), &_Vtx[0], _USAGE);
	}

public:
	unsigned int m_Size;
	unsigned int m_Count;
	unsigned int m_Offset;

	// size_t Count 점의 개수
	// size_t Size
	// const void* _Data 
	// D3D11_USAGE _USAGE 메모리가 어디 들어갈거냐?
	void Create(size_t _Count, size_t _Size, const void* _Data, D3D11_USAGE _USAGE = D3D11_USAGE::D3D11_USAGE_DEFAULT);

public:
	void Setting();

public:
	Game_VB_Buffer();
};

