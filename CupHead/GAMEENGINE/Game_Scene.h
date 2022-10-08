#pragma once
#include <Game_Mgr.h>
#include <Game_String.h>
#include <list>
#include <map>
#include <set>
#include <Object_Mgr.h>
#include "Game_Virtual_Progress.h"
#include "Game_Collision.h"

class Game_Cam;
class Game_Trans;
class Game_Renderer;
class Game_Scene_Com;
class Game_Actor;
class Game_Scene : public Game_Mgr<Game_Scene>
	, public Game_Virtual_Progress
	, Game_UpDate_ListObj_Mgr<Game_Actor>
	, Game_UpDate_ListObj_Mgr<Game_Scene_Com>
{
public:
	static Game_Ptr<Game_Scene> m_CurScene;
	static Game_Ptr<Game_Scene> m_NextScene;
	static void Progress();

	static Game_Ptr<Game_Scene> Create(const Game_String& _Name)
	{
		Game_Ptr<Game_Scene> Res = CreateToInsert(_Name);
		return Res;
	}

	template<typename STARTCOM>
	static Game_Ptr<Game_Scene> Create(const Game_String& _Name)
	{
		Game_Ptr<Game_Scene> Res = CreateToInsert(_Name);
		Res->CreateCom<STARTCOM>();
		return Res;
	}
	static void ChangeScene(const Game_String& _Name);

public:
	Game_Ptr<Game_Actor> CreateActor(const Game_String& _Name = L"");

	template<typename COMTYPE>
	Game_Ptr<COMTYPE> CreateCom(const Game_String& _Name = L"")
	{
		Game_Ptr<COMTYPE> NewCom = Object_Mgr<Game_Scene_Com>::CreateObj<COMTYPE>(_Name);
		NewCom->Scene(this);
		NewCom->Init();
		return NewCom;
	}

	template<typename COMTYPE>
	Game_Ptr<COMTYPE> FindCom(const Game_String& _Name = L"")
	{
		return Object_Mgr<Game_Scene_Com>::FindObjType<COMTYPE>();
	}


public:
	void UpdatePrev() 	override;
	void Update() 	override;
	void UpdateNext() 	override;

	void RenderPrev() 	override;
	void Render();
	void RenderNext() 	override;

	void CollisionPrev() override;
	void Collision();
	void CollisionNext() override;

	void SceneChangeEnd() 	override;
	void SceneChangeStart() override;
	void Release() override;

	void ActorClear();

	//////////////////////////////////////////////// trans 관리
private:
	friend Game_Trans;
	// 이렇게 따로 모아 놓겠다.
	std::list<Game_Ptr<Game_Trans>> m_AllTrans;

	//////////////////////////////////////////////// Renderer 관리
private:
	friend Game_Cam;
	friend Game_Renderer;
	Game_Ptr<Game_Cam> m_MainCam;
	std::map<int, Game_Ptr<Game_Cam>> m_AllCam;
	std::map<int, std::list<Game_Ptr<Game_Renderer>>> m_AllRender;

	void PushCam(Game_Ptr<Game_Cam> _Cam);
	void PushRender(Game_Ptr<Game_Renderer> _Render);

	static bool ZSORT(Game_Ptr<Game_Renderer>& _Left, Game_Ptr<Game_Renderer>& _Right);

public:
	Game_Ptr<Game_Cam> MainCam() {
		return m_MainCam;
	}

////////////////////////////////////////// Col 관리

private:
	friend Game_Collision;
	//        플레이어    
	std::map<int, std::set<int>> m_ColLink;
	std::map<int, std::list<Game_Ptr<Game_Collision>>> m_AllCol;

public:
	void PushCol(Game_Ptr<Game_Collision> _Col);

public:
	template<typename ... REST>
	void ColLink(int _Order, REST ... _Arg)
	{
		if (m_ColLink.end() == m_ColLink.find(_Order))
		{
			// 찾아봐.
			m_ColLink[_Order];
		}
		ColLinkPush(m_ColLink[_Order], _Arg...);
	}

	template<typename ... REST>
	void ColLinkPush(std::set<int>& _Value, int _PushIndex, REST ... _Arg)
	{
		if (_Value.end() != _Value.find(_PushIndex))
		{
			AMSG(L"같은 그룹과 두번 충돌하려고 했습니다.");
		}

		_Value.insert(_PushIndex);
		ColLinkPush(_Value, _Arg...);
	}

	void ColLinkPush(std::set<int>& _Value, int _PushIndex)
	{
		if (_Value.end() != _Value.find(_PushIndex))
		{
			AMSG(L"같은 그룹과 두번 충돌하려고 했습니다.");
		}

		_Value.insert(_PushIndex);
	}
public:
	Game_Scene();
	~Game_Scene();
};

