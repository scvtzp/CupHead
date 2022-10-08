#pragma once
#include <map>

#include "Game_Name.h"
#include "Game_String.h"
#include "Game_Ptr.h"
#include "Game_Debug.h"
#include "Game_Io.h"

template<typename T>
class Game_Mgr : public Game_Ref, public Game_Name
{
public:
	static std::map<Game_String, Game_Ptr<T>> m_AllOBJ;

public:
	static Game_Ptr<T> Find(const Game_String& _Name)
	{
		if (m_AllOBJ.end() == m_AllOBJ.find(_Name))
		{
			return nullptr;
		}

		return m_AllOBJ[_Name];
	}

	static void Insert(const Game_String& _Name, T* _NewObj)
	{
		if (Find(_Name) != nullptr )
		{
			AMSG(_Name + L"이름의 오브젝트가 이미지 존재합니다");
		}

		m_AllOBJ[_Name] = _NewObj;
	}

	// , 함수에 인자가 몇개가 들어올지도 모르고 
	// REST ... _Arg  어떻게 처리될지도 모른다.
	// 전달한다.

	template<typename ... REST>
	static Game_Ptr<T> CreateToInsert(const Game_String& _Name, REST ... _Arg)
	{
		if (Find(_Name) != nullptr)
		{
			AMSG(_Name + L"이름의 오브젝트가 이미지 존재합니다");
		}

		T* NewObj = new T(_Arg...);
		NewObj->NAME(_Name);
		Insert(_Name, NewObj);
		return NewObj;
	}

	static void Delete(const Game_String& _Name)
	{
		if (Find(_Name) == nullptr)
		{
			AMSG(_Name + L"이름의 오브젝트가 이미지 존재하지 않습니다.");
		}

		m_AllOBJ.erase(_Name);
		return;
	}


public:
	virtual ~Game_Mgr(){}
};

template<typename T>
std::map<Game_String, Game_Ptr<T>> Game_Mgr<T>::m_AllOBJ;

template<typename T>
class HFILEMGR : public Game_Mgr<T>
{
public:
	Game_File GameFile;

public:
	template<typename ... REST>
	static Game_Ptr<T> CreateToInsertToFile(const Game_String& _Path, REST ... _Arg)
	{
		Game_Ptr<T> NewRes = Game_Mgr<T>::CreateToInsert(Game_Io::FileName(_Path), _Arg...);
		NewRes->GameFile.SetPath(_Path);
		return NewRes;
	}
	template<typename ... REST>
	static Game_Ptr<T> CreateToInsertToFileObj(const Game_File& _File, REST ... _Arg)
	{
		Game_Ptr<T> NewRes = Game_Mgr<T>::CreateToInsert(_File.FileName(), _Arg...);
		NewRes->GameFile = _File;
		return NewRes;
	}

};

template<typename T>
class HFOBJMGR : public Game_Mgr<T>
{

private:
	static T* FOBJ;



public:
	static T* MAINOBJ()
	{
		return FOBJ;
	}

public:
	template<typename ... REST>
	static Game_Ptr<T> CreateToInsert(const Game_String& _Name, REST ... _Arg)
	{
		Game_Ptr<T> NewObj = Game_Mgr<T>::CreateToInsert(_Name, _Arg...);
		if (nullptr == FOBJ)
		{
			FOBJ = NewObj;
		}

		return NewObj;
	}


};

template<typename T>
T* HFOBJMGR<T>::FOBJ = nullptr;