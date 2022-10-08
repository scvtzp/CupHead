#include "Game_Io.h"
#include <Windows.h>
#include <assert.h>
#include <iostream> 
#include <io.h> 

//////////////////////////////////////////////////////////// Dir;
Game_Directory::Game_Directory()
{
	// ���� ����Ǵ� ����� �˷��ش�.
	// LP -> ������
	// STR -> ���ڿ�
	// W -> ���̵� ����Ʈ ���ڿ�
	// LPWSTR typedef

	WCHAR Str[256];
	// D:\AR28\AR28API\AR28API\HGAMEBASE\BIN\Debug\x64
	// D:\AR28\AR28API\AR28API\HGAMEBASE\HGAMECLIENT
	GetCurrentDirectory(256, Str);

	m_Path.m_Str = Str;


}
Game_Directory::~Game_Directory()
{

}

void Game_Directory::MoveParent()
{
	// ��� ���� for�� ������ �Ҽ��� �ִµ�
	// ��� �̿��ؼ� ����ڴ�.
	// �Լ� ã�� 
	// ��� �ٲٴ���?
	// ����� ������? \�� �̷���� �ִ�.
	size_t Start = m_Path.m_Str.FindLast(L"\\");
	size_t End = m_Path.m_Str.StrCount();
	m_Path.m_Str.EraseStr(Start, End);
}

void Game_Directory::MoveParent(const Game_String& _Folder)
{
	// ~~~~����
	// �ݺ����� �� ��� �մϱ�

	while (true)
	{

		Game_String Folder = FolderName();

		size_t S1 = Folder.StrCount();
		size_t S2 = _Folder.StrCount();

		if (Folder != _Folder)
		{
			MoveParent();
		}
		else {
			break;
		}
	}
}

// ���� �̸� �˷��ִ� ��
Game_String Game_Directory::FolderName()
{
	return IOName();
}

// �ڽ��� �Ʒ��ʿ� � ��ΰ� �������� �ű�� �̵��ϴ� ����̴�.
void Game_Directory::Move(const Game_String& _Folder)
{
	Game_String Path = m_Path.m_Str;

	Path += L"\\" + _Folder;
	// �� ��ΰ� ���� �����ϴ� ������� Ȯ���غ��� �Ѵ�.
	if (false == Game_Path::IsExits(Path))
	{
		// �׳� ��Ʈ����.
		assert(false);
	}

	m_Path.m_Str = Path;
}

std::list<Game_File> Game_Directory::DirAllFile(const wchar_t* _Ext, const wchar_t* _Name)
{
	std::list<Game_File> AllFileList;

	int checkDirFile = 0;
	// + L"\\*.*" ���� ��ȣ��?
	// C://*.*
	// C:// ����̺��� * ��� ���ϸ� . * ��� Ȯ���ڸ� ã�ƶ�.
	Game_String dirPath;

	Game_String Ext = _Ext;
	Game_String Name = _Name;

	if (Name != L"")
	{
		// c:\\AAA*
		dirPath = m_Path.m_Str + L"\\" + Name + L"*";
	}
	else {
		dirPath = m_Path.m_Str + L"\\*";
	}

	if (Ext != L"")
	{
		dirPath += L"." + Ext;
	}
	else {
		dirPath += L".*";
	}

	_wfinddata64i32_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
	intptr_t handle;

	// ������ ������ ã�´�.
	// ���ϵ� �ڵ��̴�.
	if ((handle = _wfindfirst(dirPath, &fd)) == -1L) //fd ����ü �ʱ�ȭ.
	{
		return AllFileList;
	}

	do //���� Ž�� �ݺ� ����
	{
		// ��༮�̸� 
		if (fd.attrib & _A_SUBDIR)
		{
			// ���丮��
			checkDirFile = 0;
		}
		else 
		{
			// �����̴�.
			checkDirFile = 1;
		}


		// ���� ���丮�� ��찡 �ʿ� ����.
		//if (checkDirFile == 0 && fd.name[0] != '.') {
		//	// cout << "Dir  : " << path << "\\" << fd.name << endl;
		//	// searchingDir(path + "\\" + fd.name);//����� ȣ��
		//}
		// 1�� �����϶��� 

		if (fd.name[0] == L'.')
		{
			continue;
		}

		if (checkDirFile == 1) 
		{
			Game_String FilePath = m_Path.m_Str + L"\\" + fd.name;

			Game_File NewFile = Game_File(FilePath);

			AllFileList.push_back(NewFile);
		}

		// ���� ������ ������ -1�ΰ��� ������ ���̴�.
	} while (_wfindnext(handle, &fd) == 0);

	_findclose(handle);


	return AllFileList;
}

Game_String Game_File::Ext() const
{
	size_t Start = m_Path.m_Str.FindLast(L".");
	size_t End = m_Path.m_Str.StrCount();
	// ������ ���� �����?
	return m_Path.m_Str.RangeToStr(Start, End);
}