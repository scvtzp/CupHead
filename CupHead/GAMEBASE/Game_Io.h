#pragma once
#include "Game_String.h"
#include <list>
// 디버깅
// 파일입출력
// 문자열 
// 파일 입출력과 문자열은 같이 엮일때가 많다.

// 경로에 필요한 기능들을 담당하는 클래스

// 로드할때 이 경로 찾아야 할것 아닌가?
// 그런데 게임을 설칠하는곳이 항상 같나?
// 달라져도 그때그때 마다의 경로를 찾아가야 한다.
// D:\AR28\AR28API\AR28API\HGAMEBASE\BIN\RES
class Game_Path
{
public:
	Game_String m_Str;

public:
	// 그 경로가 존재하느냐 안하느냐?
	static bool IsExits(const Game_String& _Str)
	{
		// 0 그파일이 있느냐 없느냐?
		// 2 그 파일이 읽기 전용이냐 아니냐?
		if (0 == _waccess_s(_Str, 0))
		{
			return true;
		}

		return false;
	}

public:
	Game_Path() {

	}


	Game_Path(const wchar_t* _Path)  : m_Str(_Path)
	{

	}

	Game_Path(const Game_String& _Path) : m_Str(_Path)
	{

	}
	~Game_Path() {

	}
};

class Game_Io 
{
protected:
	Game_Path m_Path;

public:
	Game_String PlusFileName(const Game_String& _Path)
	{
		return m_Path.m_Str + L"\\" + _Path;
	}

	static Game_String FileName(const Game_String& _Path) 
	{
		size_t Start = _Path.FindLast(L"\\");
		size_t End = _Path.StrCount();
		// 마지막 폴더 경로죠?
		return _Path.RangeToStr(Start, End);
	}


	const wchar_t* PCONSTWCHAR() 
	{
		return m_Path.m_Str;
	}

	Game_String IOName() const
	{
		size_t Start = m_Path.m_Str.FindLast(L"\\");
		size_t End = m_Path.m_Str.StrCount();
		// 마지막 폴더 경로죠?
		return m_Path.m_Str.RangeToStr(Start, End);
	}

	void SetPath(const Game_String& _Path)
	{
		m_Path = _Path;
	}

	Game_Io() {

	}
};


// 둘다 경로를 잘 들고 있다.
// 디렉토리
class Game_File;
class Game_Directory : public Game_Io
{
public:
	void MoveParent();
	// 요이름의 폴더가 있을때까지 올라가라.
	void MoveParent(const Game_String& _Folder);
	void Move(const Game_String& _Folder);

	std::list<Game_File> DirAllFile(const wchar_t* _Ext = L"", const wchar_t* _Name = L"");


	Game_String FolderName();

	// 이녀석은 생성되면
	// 무조건 m_Path를 지금 실행되는 경로를 자신의 경로로 삼게 만든다.
public:
	Game_Directory();
	~Game_Directory();
};

// 파일
class Game_File : public Game_Io
{
public:
	Game_String FullPath() const
	{
		return m_Path.m_Str;
	}

	Game_String FileName() const
	{
		return IOName();
	}

	Game_String Ext() const;



public:
	Game_File() {

	}

	Game_File(const Game_String& _FilePath)
	{
		m_Path = _FilePath;
	}

	Game_File(const wchar_t* _Path)
	{
		m_Path = _Path;
	}
	~Game_File() {

	}
};
