#pragma once
#include <list>
#include "Game_Ptr.h"
#include "Game_String.h"


// 실행도중 일반적으로 알수 없는 타입을
// 알수있게 해주는 기술을
// RTTI REAL TIME TYPE INFOMATION


template<typename MGRTYPE>
class Object_Mgr
{
	// 이름으로도 찾고
	// 타입으로 찾고
	// 순서

public:
	class LISTDATA 
	{
	public:
		Game_String Name;
		const type_info* Type;
		Game_Ptr<MGRTYPE> Obj;
	};


protected:
	// 다형성을 쓰겠다.
	std::list<LISTDATA> m_ObjList;

public:
	Object_Mgr() {}
	virtual ~Object_Mgr() {}

public:
	template<typename CREATETYPE>
	Game_Ptr<CREATETYPE> CreateObj(const Game_String& _Name = L"")
	{
		LISTDATA DATA;
		DATA.Name = _Name;
		DATA.Type = &typeid(CREATETYPE);

		// Game_Ptr<Game_Scene_Com> Obj => Game_Ptr<HTITLECOM>;
		CREATETYPE* NewPtr = new CREATETYPE();
		DATA.Obj = NewPtr;
		m_ObjList.push_back(DATA);
		return NewPtr;
	}


	template<typename FINDTYPE>
	FINDTYPE* FindObjName(std::wstring _Name)
	{
		for (auto& _Value : m_ObjList)
		{
			if (_Value.Name == _Name)
			{
				if (_Value.Type != &typeid(FINDTYPE))
				{
					return nullptr;
				}

				return (FINDTYPE*)_Value.Obj;
			}
		}

		return nullptr;
	}


	template<typename FINDTYPE>
	Game_Ptr<FINDTYPE> FindObjType()
	{
		// RTTI를 통해서 서로가 같은 타입인지를 비교해내는것이다.
		for (auto& _Value : m_ObjList)
		{
			if (_Value.Type == &typeid(FINDTYPE))
			{
				// _Value.Obj의 타입

				return _Value.Obj;
			}
		}
		return nullptr;
	}

	template<typename FINDTYPE>
	std::list<Game_Ptr<FINDTYPE>> FindObjTypeAll()
	{
		std::list<Game_Ptr<FINDTYPE>> List;
		for (auto& _Value : m_ObjList)
		{
			if (_Value.Type == &typeid(FINDTYPE))
			{
				List.push_back((FINDTYPE*)_Value.Obj);
			}
		}
		return List;
	}

};


template<typename MGRTYPE>
class Game_UpDate_ListObj_Mgr : public Object_Mgr<MGRTYPE>
{
public:
	void UpdatePrev() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->UpdatePrev();
		}
	}
	void Update() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->Update();
		}
	}
	void UpdateNext() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->UpdateNext();
		}
	}
	void RenderPrev() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->RenderPrev();
		}

	}
	void RenderNext() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->RenderNext();
		}

	}
	void CollisionPrev() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->CollisionPrev();
		}

	}
	void CollisionNext() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->CollisionNext();
		}
	}
	void SceneChangeEnd() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->SceneChangeEnd();
		}
	}
	void SceneChangeStart() 
	{
		for (auto& Value : Object_Mgr<MGRTYPE>::m_ObjList)
		{
			if (false == Value.Obj->IsUpdate())
			{
				continue;
			}
			Value.Obj->SceneChangeStart();
		}

	}
	void Release() 
	{
	}
};
