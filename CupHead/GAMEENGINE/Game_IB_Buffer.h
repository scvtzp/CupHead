#pragma once
// 점 RECT
// 집합 다수 => 자료구조
#include <vector>
#include <Game_Mgr.h>
#include "Game_Device_Buffer.h"
#include <Game_Ptr.h>
// 포지션



class Game_IB_Buffer : public Game_Mgr<Game_IB_Buffer>, public Game_Device_Buffer
{
public:
	template<typename T>
	static void Create(const Game_String& _Name, std::vector<T> _Idx, D3D11_USAGE _USAGE = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		Game_Ptr<Game_IB_Buffer> Res = CreateToInsert(_Name);

		DXGI_FORMAT Fmt;

		if (2 == sizeof(T))
		{
			Fmt = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		}
		else if(4 == sizeof(T)) {
			Fmt = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}
		Res->Create(_Idx.size(), sizeof(T), &_Idx[0], Fmt, _USAGE);
	}

private:
	DXGI_FORMAT m_Fmt;
	unsigned int m_Size;
	unsigned int m_Count;
	unsigned int m_Offset;

public:
	unsigned int Count() 
	{
		return m_Count;
	}

public:
	// size_t Count 점의 개수
	// size_t Size
	// const void* _Data 
	// D3D11_USAGE _USAGE 메모리가 어디 들어갈거냐?
	void Create(size_t _Count, size_t _Size, const void* _Data, DXGI_FORMAT _Fmt, D3D11_USAGE _USAGE = D3D11_USAGE::D3D11_USAGE_DEFAULT);

public:
	void Setting();

public:
	Game_IB_Buffer();
	~Game_IB_Buffer();
};

