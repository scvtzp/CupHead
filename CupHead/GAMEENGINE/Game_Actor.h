#pragma once
#include <Object_Mgr.h>
#include "Game_Virtual_Progress.h"

// 이녀석은 기능을 관리하는 기능만 존재한다.
// 이 엑터는 주인공이 될꺼야.
class Game_Trans;
class Game_Actor_Com;
class Game_Scene;
class Game_Actor : public Game_Ref, public Game_Virtual_Progress, public Game_UpDate_ListObj_Mgr<Game_Actor_Com>
{
public:
	template< typename _Type > 
	friend class Game_UpDate_ListObj_Mgr;

	friend Game_Scene;
	friend Game_Trans;

public:
	template<typename COMTYPE, typename ... REST>
	Game_Ptr<COMTYPE> CreateComName(const Game_String& _Name = L"", REST ... _ARG)
	{
		Game_Ptr<COMTYPE> NewCom = Object_Mgr<Game_Actor_Com>::CreateObj<COMTYPE>(_Name);
		NewCom->ACTOR(this);
		NewCom->Init(_ARG...);
		return NewCom;
	}

	template<typename COMTYPE, typename ... REST>
	Game_Ptr<COMTYPE> CreateCom(REST ... _ARG)
	{
		Game_Ptr<COMTYPE> NewCom = Object_Mgr<Game_Actor_Com>::CreateObj<COMTYPE>();
		NewCom->ACTOR(this);
		NewCom->Init(_ARG...);
		return NewCom;
	}

private:
	Game_Scene* m_SCENE;
	Game_Trans* m_TRANS;

public:
	Game_Trans* TRANS();
	Game_Scene* SCENE();

public:
	template<typename FINDTYPE>
	FINDTYPE* FindComName(std::wstring _Name)
	{
		return FindObjName<FINDTYPE>(_Name);
	}


	template<typename FINDTYPE>
	Game_Ptr<FINDTYPE> FindComType()
	{
		return FindObjType<FINDTYPE>();
	}

	template<typename FINDTYPE>
	std::list<Game_Ptr<FINDTYPE>> FindComTypeAll()
	{
		return FindObjTypeAll<FINDTYPE>();
	}

private:
	void UpdatePrev() 	override;
	void Update() 	override;
	void UpdateNext() 	override;
	void RenderPrev() 	override;
	void RenderNext() 	override;
	void CollisionPrev() override;
	void CollisionNext() override;
	void SceneChangeEnd() 	override;
	void SceneChangeStart() override;
	void Release() override;


};

