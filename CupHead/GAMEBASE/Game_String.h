#pragma once
#include <string>
#include <atlstr.h> // char => wchar wchar => char로 변환시켜주는 함수가 들어있다.

class Game_String
{
public:
	std::wstring m_Str;

public:
	// 마지막
	size_t StrCount() const
	{
		return m_Str.size();
	}

public:
	operator const wchar_t*() const
	{
		// 내부에 있는 배열의 포인터를 리턴해주 준다.
		return m_Str.c_str();
	}

	operator std::wstring () const
	{
		// 내부에 있는 배열의 포인터를 리턴해주 준다.
		return m_Str;
	}

	operator std::string() const
	{
		return CW2A(m_Str.c_str()).m_psz;

		// 내부에 있는 배열의 포인터를 리턴해주 준다.
		// return m_Str.c_str();
	}

	operator char*() const
	{
		return CW2A(m_Str.c_str()).m_psz;

		// 내부에 있는 배열의 포인터를 리턴해주 준다.
		// return m_Str.c_str();
	}


	bool operator==(const wchar_t* _Other) const
	{
		return m_Str == _Other;
	}

	bool operator!=(const wchar_t* _Other) const
	{
		return m_Str != _Other;
	}

	bool operator==(const Game_String& _Other) const
	{
		return _Other.m_Str == m_Str;
	}

	bool operator!=(const Game_String& _Other) const
	{
		return _Other.m_Str != m_Str;
	}

	bool operator>(const Game_String& _Other) const
	{
		return _Other.m_Str > m_Str;
	}

	bool operator<(const Game_String& _Other) const
	{
		return _Other.m_Str < m_Str;
	}


	Game_String& operator=(const wchar_t* _Ptr) 
	{
		m_Str = _Ptr;

		return *this;
	}

	Game_String& operator=(const Game_String& _Other)
	{
		m_Str = _Other.m_Str;

		return *this;
	}

	Game_String operator+(const wchar_t* _Other) const 
	{
		return m_Str + _Other;
	}

	Game_String operator+(const Game_String& _Other) const
	{
		return m_Str + _Other.m_Str;
	}

	Game_String& operator+=(const Game_String& _Other)
	{
		m_Str += _Other.m_Str;
		return *this;
	}

	size_t Find(const Game_String& _Find) const
	{
		// find라는 함수가 wstring
		// wchar_t*
		return m_Str.find(_Find.m_Str);
	}

	size_t FindLast(const Game_String& _Find) const
	{
		// Last 뒤에서 부터 찾는다.
		return m_Str.find_last_of(_Find.m_Str);
	}

	// 지워라
	void EraseStr(size_t _Start, size_t _End)
	{
		// m_Str wstring
		// wstring 내부의 기능들을 이용해서 만들고 있다.
		// Last 뒤에서 부터 찾는다.
		m_Str.replace(_Start, _End, L"");
	}

	// 일부분만 문자열에서 뜯어내는 기능
	Game_String RangeToStr(size_t _Start, size_t _End) const
	{
		// 애를 벡터라고 봤을때
		std::wstring Str;
		// 미리 버퍼를 할당하는것
		Str.resize(_End - _Start - 1);

		// 어디서부터 어디까지다.
		m_Str.copy(&Str[0], _End - _Start - 1, _Start + 1);
		return Str; // '\\' 원하지 않는다
	}

public:
	Game_String() : m_Str()
	{
	}

	Game_String(const char* _Ptr) : m_Str(CA2W(_Ptr).m_psz)
	{
	}

	Game_String(const wchar_t* _Ptr) : m_Str(_Ptr)
	{
	}

	Game_String(std::wstring _Str) : m_Str(_Str)
	{
	}
};


// 전역 오퍼레이팅
// 헤더에다가는 전역함수 구현해놓으면 안된다.
Game_String operator+(const wchar_t* _Left, const Game_String& _Right);
bool operator==(const wchar_t* _Left, const Game_String& _Right);
bool operator!=(const wchar_t* _Left, const Game_String& _Right);