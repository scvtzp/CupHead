#pragma once
#include <string>
#include <atlstr.h> // char => wchar wchar => char�� ��ȯ�����ִ� �Լ��� ����ִ�.

class Game_String
{
public:
	std::wstring m_Str;

public:
	// ������
	size_t StrCount() const
	{
		return m_Str.size();
	}

public:
	operator const wchar_t*() const
	{
		// ���ο� �ִ� �迭�� �����͸� �������� �ش�.
		return m_Str.c_str();
	}

	operator std::wstring () const
	{
		// ���ο� �ִ� �迭�� �����͸� �������� �ش�.
		return m_Str;
	}

	operator std::string() const
	{
		return CW2A(m_Str.c_str()).m_psz;

		// ���ο� �ִ� �迭�� �����͸� �������� �ش�.
		// return m_Str.c_str();
	}

	operator char*() const
	{
		return CW2A(m_Str.c_str()).m_psz;

		// ���ο� �ִ� �迭�� �����͸� �������� �ش�.
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
		// find��� �Լ��� wstring
		// wchar_t*
		return m_Str.find(_Find.m_Str);
	}

	size_t FindLast(const Game_String& _Find) const
	{
		// Last �ڿ��� ���� ã�´�.
		return m_Str.find_last_of(_Find.m_Str);
	}

	// ������
	void EraseStr(size_t _Start, size_t _End)
	{
		// m_Str wstring
		// wstring ������ ��ɵ��� �̿��ؼ� ����� �ִ�.
		// Last �ڿ��� ���� ã�´�.
		m_Str.replace(_Start, _End, L"");
	}

	// �Ϻκи� ���ڿ����� ���� ���
	Game_String RangeToStr(size_t _Start, size_t _End) const
	{
		// �ָ� ���Ͷ�� ������
		std::wstring Str;
		// �̸� ���۸� �Ҵ��ϴ°�
		Str.resize(_End - _Start - 1);

		// ��𼭺��� ��������.
		m_Str.copy(&Str[0], _End - _Start - 1, _Start + 1);
		return Str; // '\\' ������ �ʴ´�
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


// ���� ���۷�����
// ������ٰ��� �����Լ� �����س����� �ȵȴ�.
Game_String operator+(const wchar_t* _Left, const Game_String& _Right);
bool operator==(const wchar_t* _Left, const Game_String& _Right);
bool operator!=(const wchar_t* _Left, const Game_String& _Right);