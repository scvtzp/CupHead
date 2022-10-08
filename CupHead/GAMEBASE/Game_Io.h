#pragma once
#include "Game_String.h"
#include <list>
// �����
// ���������
// ���ڿ� 
// ���� ����°� ���ڿ��� ���� ���϶��� ����.

// ��ο� �ʿ��� ��ɵ��� ����ϴ� Ŭ����

// �ε��Ҷ� �� ��� ã�ƾ� �Ұ� �ƴѰ�?
// �׷��� ������ ��ĥ�ϴ°��� �׻� ����?
// �޶����� �׶��׶� ������ ��θ� ã�ư��� �Ѵ�.
// D:\AR28\AR28API\AR28API\HGAMEBASE\BIN\RES
class Game_Path
{
public:
	Game_String m_Str;

public:
	// �� ��ΰ� �����ϴ��� ���ϴ���?
	static bool IsExits(const Game_String& _Str)
	{
		// 0 �������� �ִ��� ������?
		// 2 �� ������ �б� �����̳� �ƴϳ�?
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
		// ������ ���� �����?
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
		// ������ ���� �����?
		return m_Path.m_Str.RangeToStr(Start, End);
	}

	void SetPath(const Game_String& _Path)
	{
		m_Path = _Path;
	}

	Game_Io() {

	}
};


// �Ѵ� ��θ� �� ��� �ִ�.
// ���丮
class Game_File;
class Game_Directory : public Game_Io
{
public:
	void MoveParent();
	// ���̸��� ������ ���������� �ö󰡶�.
	void MoveParent(const Game_String& _Folder);
	void Move(const Game_String& _Folder);

	std::list<Game_File> DirAllFile(const wchar_t* _Ext = L"", const wchar_t* _Name = L"");


	Game_String FolderName();

	// �̳༮�� �����Ǹ�
	// ������ m_Path�� ���� ����Ǵ� ��θ� �ڽ��� ��η� ��� �����.
public:
	Game_Directory();
	~Game_Directory();
};

// ����
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
